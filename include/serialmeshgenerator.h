#ifndef SERIALMESHGENERATOR_H
#define SERIALMESHGENERATOR_H

#include "IMeshGenerator.h"
#include <vtkMarchingCubes.h>

class SerialMeshGenerator : public IMeshGenerator {
public:
    vtkSmartPointer<vtkPolyData> generateMesh(vtkSmartPointer<vtkImageData> vtkImage, double isoValue) override;
};


#endif // SERIALMESHGENERATOR_H
