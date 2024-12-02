#ifndef FILEMANAGER_H
#define FILEMANAGER_H

// Qt libraries
#include <QString>

// ITK libraries
#include <itkImage.h>
#include <itkSmartPointer.h>

// VTK libraries
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

class FileManager {
public:
    using ImageType = itk::Image<unsigned short, 3>;

    // Load a NIfTI file
    itk::SmartPointer<ImageType> loadNiftiFile(const QString &fullPath) const;

    // Save as DICOM
    bool saveAsDICOM(const itk::SmartPointer<ImageType> &image, const QString &fileName) const;

    // Save VTK Mesh
    bool saveVTKMesh(const vtkSmartPointer<vtkPolyData> &mesh, const QString &fileName) const;

private:
    // Helper method to create output directories if needed
    bool ensureOutputDirectoryExists(const QString &directoryPath) const;
};

#endif // FILEMANAGER_H
