#include "MeshGenerator.h"

// VTK libraries
#include <vtkSmoothPolyDataFilter.h>

// Qt libraries
#include <QDebug>

vtkSmartPointer<vtkPolyData> MeshGenerator::performMeshGeneration(vtkSmartPointer<vtkImageData> vtkImage, double isoValue, bool debugMode) {
    std::unique_ptr<IMeshGenerator> meshGenerator;

    // Select the appropriate mesh generator based on the debug mode
    if (debugMode) {
        qDebug() << "Using SerialMeshGenerator for mesh generation.";
        meshGenerator = std::make_unique<SerialMeshGenerator>();
    } else {
        qDebug() << "Using ParallelMeshGenerator for mesh generation.";
        meshGenerator = std::make_unique<ParallelMeshGenerator>();
    }

    // Generate the mesh using the selected generator
    vtkSmartPointer<vtkPolyData> mesh = meshGenerator->generateMesh(vtkImage, isoValue);

    // Handle the generated mesh as needed (e.g., display or save)
    if (mesh) {
        qDebug() << "Mesh generation completed successfully.";
    } else {
        qDebug() << "Mesh generation failed.";
    }

    return mesh;
}

vtkSmartPointer<vtkPolyData> MeshGenerator::smoothMesh(vtkSmartPointer<vtkPolyData> inputMesh, int iterations, double relaxationFactor) {
    // Create the smoothing filter
    vtkSmartPointer<vtkSmoothPolyDataFilter> smoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    smoother->SetInputData(inputMesh);
    smoother->SetNumberOfIterations(iterations);  // Number of smoothing iterations
    smoother->SetRelaxationFactor(relaxationFactor);  // Relaxation factor controls smoothing strength
    smoother->FeatureEdgeSmoothingOff();  // Optional: turn on if you want edge smoothing
    smoother->BoundarySmoothingOn();      // Optional: turn off if you don't want boundary smoothing
    smoother->Update();

    // Return the smoothed mesh
    return smoother->GetOutput();
}