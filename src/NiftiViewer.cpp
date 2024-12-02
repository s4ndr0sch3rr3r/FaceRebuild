#include <vtkSmartPointer.h>
#include <vtkImageSlice.h>
#include <vtkImageSliceMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <itkImage.h>
#include <itkNiftiImageIO.h>
#include <itkImageFileReader.h>
#include <itkImageToVTKImageFilter.h>

class NiftiViewer
{
public:
    void DisplayImage(const char* filename)
    {
        // Define the pixel type and dimension of the image.
        using PixelType = unsigned short;
        constexpr unsigned int Dimension = 3;
        using ImageType = itk::Image<PixelType, Dimension>;

        // Read the NIfTI image using ITK
        itk::NiftiImageIO::Pointer imageIO = itk::NiftiImageIO::New();
        itk::ImageFileReader<ImageType>::Pointer reader = itk::ImageFileReader<ImageType>::New();
        reader->SetFileName(filename);
        reader->SetImageIO(imageIO);
        reader->Update();

        // Convert the ITK image to a VTK image
        using ImageToVTKFilterType = itk::ImageToVTKImageFilter<ImageType>;
        ImageToVTKFilterType::Pointer imageToVTKFilter = ImageToVTKFilterType::New();
        imageToVTKFilter->SetInput(reader->GetOutput());
        imageToVTKFilter->Update();

        // Create a VTK image slice mapper and image slice
        vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
        imageSliceMapper->SetInputData(imageToVTKFilter->GetOutput());

        vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
        imageSlice->SetMapper(imageSliceMapper);

        // Setup the renderer, render window, and interactor
        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);

        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        renderWindowInteractor->SetRenderWindow(renderWindow);

        // Add the image slice to the renderer
        renderer->AddViewProp(imageSlice);

        // Render and interact
        renderWindow->Render();
        renderer->ResetCamera();
        renderWindow->Render();

        renderWindowInteractor->Start();
    }
};