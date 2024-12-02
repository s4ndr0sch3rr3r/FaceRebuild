#include "itktovtkconverter.h"

// ITK libraries
#include <itkImageToVTKImageFilter.h>

// Qt libraries
#include <QDebug>

template <typename TPixel, unsigned int TDimension>
vtkSmartPointer<vtkImageData> ITKtoVTKConverter::Convert(const itk::Image<TPixel, TDimension> *itkImage) {
    if (!itkImage) {
        qDebug() << "Null ITK image supplied to converter.";
        return nullptr;
    }

    using ImageType = itk::Image<TPixel, TDimension>;
    using ConverterType = itk::ImageToVTKImageFilter<ImageType>;

    typename ConverterType::Pointer converter = ConverterType::New();
    converter->SetInput(itkImage);

    try {
        converter->Update();
    } catch (const itk::ExceptionObject &ex) {
        qDebug() << "Exception caught during ITK to VTK conversion:" << ex.what();
        return nullptr;
    }

    return converter->GetOutput();
}

// Explicit instantiation
template vtkSmartPointer<vtkImageData> ITKtoVTKConverter::Convert<unsigned short, 3>(const itk::Image<unsigned short, 3>*);
