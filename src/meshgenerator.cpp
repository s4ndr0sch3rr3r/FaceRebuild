#include "MeshGenerator.h"

// VTK libraries
#include <vtkSmoothPolyDataFilter.h>

// Qt libraries
#include <QDebug>

#include <chrono>

// Perform mesh generation
vtkSmartPointer<vtkPolyData> MeshGenerator::performMeshGeneration(const vtkSmartPointer<vtkImageData> &vtkImage, double isoValue, bool debugMode) {
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

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Call the generateMesh function
    vtkSmartPointer<vtkPolyData> mesh = meshGenerator->generateMesh(vtkImage, isoValue);

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Output the time taken
    std::cout << "Mesh generation took: " << elapsed.count() << " seconds." << std::endl;

    // Log the status of the mesh generation
    if (mesh) {
        qDebug() << "Mesh generation completed successfully.";
    } else {
        qDebug() << "Mesh generation failed.";
    }

    return mesh;
}

// Smooth the input mesh
vtkSmartPointer<vtkPolyData> MeshGenerator::smoothMesh(const vtkSmartPointer<vtkPolyData> &inputMesh, int iterations, double relaxationFactor) {
    if (!inputMesh) {
        qDebug() << "Invalid input mesh. Cannot perform smoothing.";
        return nullptr;
    }

    // Create the smoothing filter
    vtkSmartPointer<vtkSmoothPolyDataFilter> smoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    smoother->SetInputData(inputMesh);
    smoother->SetNumberOfIterations(iterations);  // Number of smoothing iterations
    smoother->SetRelaxationFactor(relaxationFactor);  // Relaxation factor controls smoothing strength
    smoother->FeatureEdgeSmoothingOff();  // Optional: turn on if you want edge smoothing
    smoother->BoundarySmoothingOn();      // Optional: turn off if you don't want boundary smoothing

    try {
        smoother->Update();
    } catch (const std::exception &e) {
        qDebug() << "Error during smoothing:" << e.what();
        return nullptr;
    }

    // Return the smoothed mesh
    return smoother->GetOutput();
}
