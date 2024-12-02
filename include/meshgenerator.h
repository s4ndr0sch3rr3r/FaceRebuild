#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

#include "IMeshGenerator.h"
#include "SerialMeshGenerator.h"
#include "ParallelMeshGenerator.h"

// VTK libraries
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>

#include <memory>
#include <QDebug>

class MeshGenerator {
public:
    vtkSmartPointer<vtkPolyData> performMeshGeneration(vtkSmartPointer<vtkImageData> vtkImage, double isoValue, bool debugMode);
    vtkSmartPointer<vtkPolyData> smoothMesh(vtkSmartPointer<vtkPolyData> inputMesh, int iterations = 30, double relaxationFactor = 0.1);
};

#endif // MESHGENERATOR_H
