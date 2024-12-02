#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkImageSliceMapper.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <QVTKOpenGLNativeWidget.h>
#include <QTimer>

#include <ScrollInteractorStyle.h>

class RendererManager {
public:
    RendererManager();

    void setupRenderer(QVTKOpenGLNativeWidget *widget);
    void setupVTKWidget(const vtkSmartPointer<vtkImageData> &vtkImage); // Full widget setup method
    void enableSliceInteraction();
    void disableSliceInteraction();
    void enableStyle3DInteraction();
    void disableStyle3DInteraction();
    void OnScrollForward();
    void OnScrollBackward();
    void updateRenderer();
    void clearRendererActors();
    vtkSmartPointer<vtkImageData> createTestImage() const;
    void displayMesh(const vtkSmartPointer<vtkPolyData> &mesh);

private:
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkImageActor> m_imageActor;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
    vtkSmartPointer<vtkImageSliceMapper> m_sliceMapper;
    QVTKOpenGLNativeWidget *m_vtkWidget;

    QTimer m_renderTimer;
    int m_currentSlice;
    int m_maxSlices;
    bool m_needRender;
};

#endif // RENDERERMANAGER_H
