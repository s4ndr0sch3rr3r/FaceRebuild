#include "serialmeshgenerator.h"
#include <vtkMarchingCubes.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>

vtkSmartPointer<vtkPolyData> SerialMeshGenerator::generateMesh(vtkSmartPointer<vtkImageData> vtkImage, double isoValue) {
    // Create and set up the marching cubes algorithm for serial execution
    vtkSmartPointer<vtkMarchingCubes> mc = vtkSmartPointer<vtkMarchingCubes>::New();
    mc->SetInputData(vtkImage);
    mc->SetValue(0, isoValue);
    mc->Update();

    // Return the generated mesh as vtkPolyData
    return mc->GetOutput();
}
