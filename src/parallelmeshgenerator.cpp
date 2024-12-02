#include "parallelmeshgenerator.h"
#include <vtkFlyingEdges3D.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <QDebug>

// Generate a 3D mesh using the Flying Edges algorithm
vtkSmartPointer<vtkPolyData> ParallelMeshGenerator::generateMesh(const vtkSmartPointer<vtkImageData> &vtkImage, double isoValue) {
    if (!vtkImage) {
        qDebug() << "Invalid input image. Cannot generate mesh.";
        return nullptr;
    }

    // Create and set up the Flying Edges algorithm for parallel execution
    vtkSmartPointer<vtkFlyingEdges3D> fe = vtkSmartPointer<vtkFlyingEdges3D>::New();
    fe->SetInputData(vtkImage);
    fe->SetValue(0, isoValue);

    try {
        fe->Update();
    } catch (const std::exception &e) {
        qDebug() << "Error during mesh generation:" << e.what();
        return nullptr;
    }

    // Log the successful completion of mesh generation
    qDebug() << "Mesh generation using Flying Edges completed successfully.";

    // Return the generated mesh as vtkPolyData
    return fe->GetOutput();
}
