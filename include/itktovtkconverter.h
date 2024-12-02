#ifndef ITKTOVTKCONVERTER_H
#define ITKTOVTKCONVERTER_H

// ITK libraries
#include <itkImage.h>

// VTK libraries
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

// Converts ITK images to VTK images
class ITKtoVTKConverter {
public:
    // Convert ITK image to VTK image
    template <typename TPixel, unsigned int TDimension>
    static vtkSmartPointer<vtkImageData> Convert(const itk::Image<TPixel, TDimension> *itkImage);
};

// Declare explicit instantiation for common use case
extern template vtkSmartPointer<vtkImageData> ITKtoVTKConverter::Convert<unsigned short, 3>(const itk::Image<unsigned short, 3>*);

#endif // ITKTOVTKCONVERTER_H
