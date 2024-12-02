#include "mainwindow.h"
#include "filemanager.h"
#include "itktovtkconverter.h"
#include "imageprocessor.h"
#include "meshgenerator.h"

// Qt libraries
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QSpinBox>
#include <QStatusBar>

// VTK libraries
#include <vtkGenericOpenGLRenderWindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_vtkWidget(new QVTKOpenGLNativeWidget(this)),
    m_generateButton(nullptr), m_currentSlice(0), m_maxSlices(0), m_needRender(false) {

    setWindowTitle("VTK Viewer");
    resize(800, 600);
    setCentralWidget(m_vtkWidget);

    m_rendererManager.setupRenderer(m_vtkWidget);
    createMenu();
    addControls();
}

MainWindow::~MainWindow() {}

void MainWindow::createMenu() {
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *openAction = new QAction("&Open NIfTI", this);
    fileMenu->addAction(openAction);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
}

void MainWindow::addControls() {
    m_generateButton = new QPushButton("Generate", this);
    statusBar()->addWidget(m_generateButton);
    connect(m_generateButton, &QPushButton::clicked, this, &MainWindow::generate);

    QWidget *thresholdControlWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(thresholdControlWidget);

    QLabel *lowLabel = new QLabel("Low threshold-value", thresholdControlWidget);
    layout->addWidget(lowLabel);

    m_lowSpinBox = new QSpinBox(thresholdControlWidget);
    m_lowSpinBox->setRange(1, 254);
    m_lowSpinBox->setSingleStep(5);
    m_lowSpinBox->setValue(40);
    layout->addWidget(m_lowSpinBox);

    QLabel *highLabel = new QLabel("High threshold-value", thresholdControlWidget);
    layout->addWidget(highLabel);

    m_highSpinBox = new QSpinBox(thresholdControlWidget);
    m_highSpinBox->setRange(2, 255);
    m_highSpinBox->setSingleStep(5);
    m_highSpinBox->setValue(100);
    layout->addWidget(m_highSpinBox);

    thresholdControlWidget->setLayout(layout);
    statusBar()->addPermanentWidget(thresholdControlWidget);
}

void MainWindow::openFile() {
    QString initialDir = QDir(QCoreApplication::applicationDirPath()).filePath("../../Data");
    QString fileName = QFileDialog::getOpenFileName(this, "Open NIfTI File", initialDir, "NIfTI Files (*.nii *.nii.gz)");

    if (!fileName.isEmpty()) {
        displayNiftiImage(fileName);
    }
}

void MainWindow::generate() {
    if (!m_itkImage) {
        qDebug() << "No ITK image loaded.";
        return;
    }

    ImageProcessor processor;

    auto filteredImage = processor.filter(m_itkImage);
    if (!filteredImage) return;

    int lowValue = m_lowSpinBox->value();
    int highValue = m_highSpinBox->value();
    auto thresholdedImage = processor.threshold(filteredImage, lowValue, highValue);
    if (!thresholdedImage) return;

    if (m_debugOn) {
        FileManager fileManager;
        fileManager.saveAsDICOM(thresholdedImage, "afterThresholding.dcm");
    }

    auto vtkImage = ITKtoVTKConverter::Convert<unsigned short, 3>(thresholdedImage);
    if (!vtkImage) return;

    MeshGenerator meshGen;
    auto mesh = meshGen.performMeshGeneration(vtkImage, 0.5, false);
    if (mesh) displayMesh(meshGen.smoothMesh(mesh));
}

void MainWindow::displayMesh(const vtkSmartPointer<vtkPolyData> &mesh) {
    m_rendererManager.displayMesh(mesh);
}

void MainWindow::displayNiftiImage(const QString &filename) {
    m_rendererManager.clearRendererActors();

    FileManager fileManager;
    m_itkImage = fileManager.loadNiftiFile(filename);
    if (!m_itkImage) return;

    ImageProcessor processor;
    m_itkImage = processor.filter(m_itkImage);
    m_itkImage = processor.normalize(m_itkImage);
    m_itkImage = processor.resample(m_itkImage);

    auto vtkImage = ITKtoVTKConverter::Convert<unsigned short, 3>(m_itkImage);
    if (!vtkImage) return;

    m_rendererManager.setupVTKWidget(vtkImage);
    m_vtkWidget->renderWindow()->Render();
}
