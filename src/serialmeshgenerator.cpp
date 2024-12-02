#include "serialmeshgenerator.h"
#include <vtkMarchingCubes.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <QDebug>

// Generate a 3D mesh using the Marching Cubes algorithm
vtkSmartPointer<vtkPolyData> SerialMeshGenerator::generateMesh(const vtkSmartPointer<vtkImageData> &vtkImage, double isoValue) {
    if (!vtkImage) {
        qDebug() << "Invalid input image. Cannot generate mesh.";
        return nullptr;
    }

    // Create and set up the Marching Cubes algorithm for serial execution
    vtkSmartPointer<vtkMarchingCubes> mc = vtkSmartPointer<vtkMarchingCubes>::New();
    mc->SetInputData(vtkImage);
    mc->SetValue(0, isoValue);

    try {
        mc->Update();
    } catch (const std::exception &e) {
        qDebug() << "Error during mesh generation:" << e.what();
        return nullptr;
    }

    // Log the successful completion of mesh generation
    qDebug() << "Mesh generation using Marching Cubes completed successfully.";

    // Return the generated mesh as vtkPolyData
    return mc->GetOutput();
}
