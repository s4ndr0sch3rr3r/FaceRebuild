#ifndef PARALLELMESHGENERATOR_H
#define PARALLELMESHGENERATOR_H

#include "IMeshGenerator.h"
#include <vtkFlyingEdges3D.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>

// Class for generating 3D meshes in parallel using VTK's Flying Edges algorithm
class ParallelMeshGenerator : public IMeshGenerator {
public:
    // Generate a mesh from vtkImageData using the Flying Edges 3D algorithm
    vtkSmartPointer<vtkPolyData> generateMesh(const vtkSmartPointer<vtkImageData> &vtkImage, double isoValue) override;
};

#endif // PARALLELMESHGENERATOR_H
