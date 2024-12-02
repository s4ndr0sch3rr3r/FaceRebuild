#ifndef IMESHGENERATOR_H
#define IMESHGENERATOR_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>

class IMeshGenerator {
public:
    virtual ~IMeshGenerator() = default;
    virtual vtkSmartPointer<vtkPolyData> generateMesh(const vtkSmartPointer<vtkImageData> &vtkImage, double isoValue) = 0;
};

#endif // IMESHGENERATOR_H
