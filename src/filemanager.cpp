#include "filemanager.h"

// Qt libraries
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QString>

// VTK libraries
#include <vtkOBJWriter.h>
#include <vtkPolyDataWriter.h>
#include <vtkSmartPointer.h>

// ITK libraries
#include <itkImageFileReader.h>
#include <itkNiftiImageIO.h>
#include <itkGDCMImageIO.h>
#include <itkImageFileWriter.h>

itk::SmartPointer<FileManager::ImageType> FileManager::loadNiftiFile(const QString &fullPath) const {
    qDebug() << "Loading NIfTI file from:" << fullPath;

    auto imageIO = itk::NiftiImageIO::New();
    auto reader = itk::ImageFileReader<ImageType>::New();
    reader->SetFileName(fullPath.toStdString());
    reader->SetImageIO(imageIO);

    try {
        reader->Update();
    } catch (const itk::ExceptionObject &ex) {
        qDebug() << "Error loading the NIfTI file from" << fullPath << ":" << ex.what();
        return nullptr;
    }
    qDebug() << "NIfTI file loaded successfully.";
    return reader->GetOutput();
}

bool FileManager::saveAsDICOM(const itk::SmartPointer<ImageType> &image, const QString &fileName) const {
    if (!image) {
        qDebug() << "Input image is null, cannot save.";
        return false;
    }

    auto writer = itk::ImageFileWriter<ImageType>::New();
    auto gdcmIO = itk::GDCMImageIO::New();

    QString execDir = QCoreApplication::applicationDirPath();
    QDir dir(execDir);
    dir.cdUp();
    dir.cdUp();
    dir.cd("Data");
    dir.cd("out");

    if (!ensureOutputDirectoryExists(dir.absolutePath())) {
        return false;
    }

    QString fullPath = dir.filePath(fileName);
    writer->SetFileName(fullPath.toStdString());
    writer->SetImageIO(gdcmIO);
    writer->SetInput(image);

    try {
        writer->Update();
        qDebug() << "DICOM file saved successfully to" << fullPath;
    } catch (const itk::ExceptionObject &error) {
        qDebug() << "Error saving the DICOM file:" << QString::fromStdString(error.GetDescription());
        return false;
    }

    return true;
}

bool FileManager::saveVTKMesh(const vtkSmartPointer<vtkPolyData> &mesh, const QString &fileName) const {
    if (!mesh) {
        qDebug() << "No mesh data provided to save.";
        return false;
    }

    QString execDir = QCoreApplication::applicationDirPath();
    QDir dir(execDir);
    dir.cdUp();
    dir.cdUp();
    dir.cd("Data");
    dir.cd("out");

    if (!ensureOutputDirectoryExists(dir.absolutePath())) {
        return false;
    }

    QString fullPath = dir.filePath(fileName);
    auto writer = vtkSmartPointer<vtkOBJWriter>::New();
    writer->SetFileName(fullPath.toStdString().c_str());
    writer->SetInputData(mesh);

    try {
        writer->Write();
        qDebug() << "OBJ file saved successfully to" << fullPath;
    } catch (const std::exception &e) {
        qDebug() << "Failed to save OBJ file:" << e.what();
        return false;
    }

    return true;
}

bool FileManager::ensureOutputDirectoryExists(const QString &directoryPath) const {
    QDir dir(directoryPath);
    if (!dir.exists() && !dir.mkpath(directoryPath)) {
        qDebug() << "Failed to create directory:" << directoryPath;
        return false;
    }
    return true;
}
