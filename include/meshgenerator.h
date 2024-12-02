#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

#include "IMeshGenerator.h"
#include "SerialMeshGenerator.h"
#include "ParallelMeshGenerator.h"

// VTK libraries
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>

// Standard libraries
#include <memory>
#include <QDebug>

// Class for generating and smoothing meshes
class MeshGenerator {
public:
    // Perform mesh generation using either serial or parallel methods
    vtkSmartPointer<vtkPolyData> performMeshGeneration(const vtkSmartPointer<vtkImageData> &vtkImage, double isoValue, bool debugMode);

    // Smooth the generated mesh using VTK's smoothing filter
    vtkSmartPointer<vtkPolyData> smoothMesh(const vtkSmartPointer<vtkPolyData> &inputMesh, int iterations = 30, double relaxationFactor = 0.1);
};

#endif // MESHGENERATOR_H
