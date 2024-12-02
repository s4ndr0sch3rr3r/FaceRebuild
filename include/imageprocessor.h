#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

// ITK libraries
#include <itkImage.h>
#include <itkSmartPointer.h>

class ImageProcessor {
public:
    using ImageType = itk::Image<unsigned short, 3>;

    itk::SmartPointer<ImageProcessor::ImageType> normalize(const itk::SmartPointer<ImageType> &image) const;

    // Resample the input image to reduce resolution
    itk::SmartPointer<ImageType> resample(const itk::SmartPointer<ImageType> &image) const;

    // Apply a binary threshold to the input image
    itk::SmartPointer<ImageType> threshold(const itk::SmartPointer<ImageType> &image, int low, int high) const;

    // Apply Gaussian filtering to the input image
    itk::SmartPointer<ImageType> filter(const itk::SmartPointer<ImageType> &image) const;
};

#endif // IMAGEPROCESSOR_H
