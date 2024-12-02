#include "parallelmeshgenerator.h"
#include <vtkFlyingEdges3D.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>

vtkSmartPointer<vtkPolyData> ParallelMeshGenerator::generateMesh(vtkSmartPointer<vtkImageData> vtkImage, double isoValue) {
    // Create and set up the flying edges algorithm for parallel execution
    vtkSmartPointer<vtkFlyingEdges3D> fe = vtkSmartPointer<vtkFlyingEdges3D>::New();
    fe->SetInputData(vtkImage);
    fe->SetValue(0, isoValue);
    fe->Update();

    // Return the generated mesh as vtkPolyData
    return fe->GetOutput();
}
