#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt libraries
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QSpinBox>
#include <QCheckBox>

// VTK libraries
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>

// ITK libraries
#include <itkImage.h>
#include <itkSmartPointer.h>

// Project-specific headers
#include "RendererManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void displayNiftiImage(const QString &filename);

private slots:
    void openFile();
    void generate();

private:
    void createMenu();
    void addControls();
    void displayMesh(const vtkSmartPointer<vtkPolyData> &mesh);

    RendererManager m_rendererManager;
    QVTKOpenGLNativeWidget *m_vtkWidget;
    QPushButton *m_generateButton;
    QTimer m_renderTimer;
    QSpinBox *m_lowSpinBox;
    QSpinBox *m_highSpinBox;
    QCheckBox *m_normalizeCheckBox;

    int m_currentSlice;
    int m_maxSlices;
    bool m_needRender;

    bool m_debugOn = false;

    itk::SmartPointer<itk::Image<unsigned short, 3>> m_itkImage;
};

#endif // MAINWINDOW_H
