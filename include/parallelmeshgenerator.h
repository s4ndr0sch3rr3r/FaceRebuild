#ifndef PARALLELMESHGENERATOR_H
#define PARALLELMESHGENERATOR_H

#include "IMeshGenerator.h"
#include <vtkFlyingEdges3D.h>

class ParallelMeshGenerator : public IMeshGenerator {
public:
    vtkSmartPointer<vtkPolyData> generateMesh(vtkSmartPointer<vtkImageData> vtkImage, double isoValue) override;
};

#endif // PARALLELMESHGENERATOR_H
