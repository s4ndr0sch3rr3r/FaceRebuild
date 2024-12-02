#ifndef SERIALMESHGENERATOR_H
#define SERIALMESHGENERATOR_H

#include "IMeshGenerator.h"
#include <vtkMarchingCubes.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>

// Class for generating 3D meshes in serial using VTK's Marching Cubes algorithm
class SerialMeshGenerator : public IMeshGenerator {
public:
    // Generate a mesh from vtkImageData using the Marching Cubes algorithm
    vtkSmartPointer<vtkPolyData> generateMesh(const vtkSmartPointer<vtkImageData> &vtkImage, double isoValue) override;
};

#endif // SERIALMESHGENERATOR_H
