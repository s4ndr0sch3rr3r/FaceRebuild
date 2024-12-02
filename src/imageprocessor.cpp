#include "imageprocessor.h"

// ITK libraries
#include <itkBinaryThresholdImageFilter.h>
#include <itkDiscreteGaussianImageFilter.h>
#include <itkIdentityTransform.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkResampleImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkGDCMImageIO.h>
#include <itkMetaDataObject.h>
#include <gdcmTag.h>
#include <gdcmDataElement.h>
#include <gdcmAttribute.h>
#include <gdcmTrace.h>

// Qt libraries
#include <QDebug>

// Standard libraries
#include <iostream> // For std::cerr

itk::SmartPointer<ImageProcessor::ImageType> ImageProcessor::normalize(const itk::SmartPointer<ImageType> &image) const {
    if (!image) {
        qDebug() << "Input image is null.";
        return nullptr;
    }

    auto rescaleFilter = itk::RescaleIntensityImageFilter<ImageType, ImageType>::New();
    rescaleFilter->SetInput(image);
    rescaleFilter->SetOutputMinimum(0);   // Minimum intensity value after normalization
    rescaleFilter->SetOutputMaximum(255); // Maximum intensity value after normalization

    try {
        rescaleFilter->Update();
    } catch (const itk::ExceptionObject &error) {
        qDebug() << "Error during normalization: " << QString::fromStdString(error.GetDescription());
        return nullptr;
    }

    return rescaleFilter->GetOutput();
}


itk::SmartPointer<ImageProcessor::ImageType> ImageProcessor::resample(const itk::SmartPointer<ImageType> &image) const {
    if (!image) {
        qDebug() << "Input image is null.";
        return nullptr;
    }

    ImageType::SpacingType outputSpacing;
    outputSpacing.Fill(1.0); // New spacing to reduce resolution

    ImageType::SizeType inputSize = image->GetLargestPossibleRegion().GetSize();
    ImageType::SpacingType inputSpacing = image->GetSpacing();
    ImageType::SizeType outputSize;

    for (unsigned int i = 0; i < 3; ++i) {
        outputSize[i] = static_cast<size_t>((inputSize[i] * inputSpacing[i]) / outputSpacing[i] + 0.5);
    }

    auto resampleFilter = itk::ResampleImageFilter<ImageType, ImageType>::New();
    resampleFilter->SetInput(image);
    resampleFilter->SetSize(outputSize);
    resampleFilter->SetOutputSpacing(outputSpacing);
    resampleFilter->SetOutputOrigin(image->GetOrigin());
    resampleFilter->SetOutputDirection(image->GetDirection());

    auto interpolator = itk::LinearInterpolateImageFunction<ImageType, double>::New();
    resampleFilter->SetInterpolator(interpolator);

    auto transform = itk::IdentityTransform<double, 3>::New();
    resampleFilter->SetTransform(transform);

    try {
        resampleFilter->Update();
    } catch (const itk::ExceptionObject &error) {
        std::cerr << "Error during resampling: " << error << std::endl;
        return nullptr;
    }

    return resampleFilter->GetOutput();
}

itk::SmartPointer<ImageProcessor::ImageType> ImageProcessor::threshold(const itk::SmartPointer<ImageType> &image, int low, int high) const {
    if (!image) {
        qDebug() << "Input image is null.";
        return nullptr;
    }

    auto thresholdFilter = itk::BinaryThresholdImageFilter<ImageType, ImageType>::New();
    thresholdFilter->SetInput(image);
    thresholdFilter->SetLowerThreshold(low);
    thresholdFilter->SetUpperThreshold(high);
    thresholdFilter->SetInsideValue(1);
    thresholdFilter->SetOutsideValue(0);

    try {
        thresholdFilter->Update();
    } catch (const itk::ExceptionObject &error) {
        qDebug() << "Error during thresholding: " << QString::fromStdString(error.GetDescription());
        return nullptr;
    }

    return thresholdFilter->GetOutput();
}

itk::SmartPointer<ImageProcessor::ImageType> ImageProcessor::filter(const itk::SmartPointer<ImageType> &image) const {
    if (!image) {
        qDebug() << "Input image is null.";
        return nullptr;
    }

    auto gaussianFilter = itk::DiscreteGaussianImageFilter<ImageType, ImageType>::New();
    gaussianFilter->SetInput(image);
    gaussianFilter->SetVariance(2.0); // Smoothing amount

    try {
        gaussianFilter->Update();
    } catch (const itk::ExceptionObject &error) {
        qDebug() << "Error during Gaussian filtering: " << QString::fromStdString(error.GetDescription());
        return nullptr;
    }

    return gaussianFilter->GetOutput();
}
