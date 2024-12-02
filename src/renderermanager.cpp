#include "RendererManager.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkCallbackCommand.h>
#include <vtkProperty.h>
#include <QDebug>

RendererManager::RendererManager()
    : m_renderer(vtkSmartPointer<vtkRenderer>::New()),
    m_vtkWidget(nullptr),
    m_currentSlice(0), m_maxSlices(0), m_needRender(false) {

    m_renderTimer.setInterval(100);
    m_renderTimer.setSingleShot(true);
    QObject::connect(&m_renderTimer, &QTimer::timeout, [this]() { updateRenderer(); });
}

void RendererManager::setupRenderer(QVTKOpenGLNativeWidget *widget) {
    m_vtkWidget = widget;
    m_vtkWidget->setRenderWindow(vtkGenericOpenGLRenderWindow::New());
    m_vtkWidget->renderWindow()->AddRenderer(m_renderer);
}

void RendererManager::setupVTKWidget(const vtkSmartPointer<vtkImageData> &vtkImage) {
    if (!m_vtkWidget || !m_vtkWidget->renderWindow()) {
        qDebug() << "Error: VTK widget or render window is not properly initialized!";
        return;
    }

    int dims[3];
    vtkImage->GetDimensions(dims);
    m_currentSlice = dims[2] / 2;
    m_maxSlices = dims[2] - 1;

    m_sliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
    m_sliceMapper->SetInputData(vtkImage);
    m_sliceMapper->SetSliceNumber(m_currentSlice);
    m_sliceMapper->SetOrientationToZ();

    vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
    imageSlice->SetMapper(m_sliceMapper);

    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->AddViewProp(imageSlice);

    m_renderer->ResetCamera();
    m_renderer->ResetCameraClippingRange();

    m_vtkWidget->renderWindow()->AddRenderer(m_renderer);

    disableStyle3DInteraction();
    enableSliceInteraction();
    m_vtkWidget->renderWindow()->Render();
}

void RendererManager::clearRendererActors() {
    if (!m_vtkWidget || !m_vtkWidget->renderWindow() || !m_vtkWidget->renderWindow()->GetRenderers()) {
        qDebug() << "Error: vtkWidget, renderWindow, or renderer collection is not initialized!";
        return;
    }

    auto renderer = m_vtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer();
    if (renderer) {
        renderer->RemoveAllViewProps();
    } else {
        qDebug() << "Error: No renderer available!";
    }
}

void RendererManager::displayMesh(const vtkSmartPointer<vtkPolyData> &mesh) {
    if (!m_renderer || !m_vtkWidget || !m_vtkWidget->renderWindow()) {
        qDebug() << "Renderer or RenderWindow not initialized";
        return;
    }

    if (mesh->GetNumberOfPoints() == 0) {
        qDebug() << "Mesh has no points.";
        return;
    }

    m_renderer->RemoveAllViewProps();
    clearRendererActors();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(mesh);
    mapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.96, 0.94, 0.88);
    actor->GetProperty()->SetOpacity(1.0);

    m_renderer->AddActor(actor);
    m_renderer->ResetCamera();

    disableSliceInteraction();
    enableStyle3DInteraction();

    m_vtkWidget->renderWindow()->Render();
    m_vtkWidget->update();
}

vtkSmartPointer<vtkImageData> RendererManager::createTestImage() const {
    vtkSmartPointer<vtkImageData> testImage = vtkSmartPointer<vtkImageData>::New();
    testImage->SetDimensions(50, 50, 50);
    testImage->AllocateScalars(VTK_UNSIGNED_SHORT, 1);

    int dims[3];
    testImage->GetDimensions(dims);

    int centerX = dims[0] / 2, centerY = dims[1] / 2, centerZ = dims[2] / 2, sphereRadius = 10;

    for (int z = 0; z < dims[2]; ++z) {
        for (int y = 0; y < dims[1]; ++y) {
            for (int x = 0; x < dims[0]; ++x) {
                unsigned short *pixel = static_cast<unsigned short *>(testImage->GetScalarPointer(x, y, z));
                unsigned short intensity = static_cast<unsigned short>(z * 5);

                int dx = x - centerX, dy = y - centerY, dz = z - centerZ;
                if (std::sqrt(dx * dx + dy * dy + dz * dz) < sphereRadius) {
                    intensity = 500;
                }

                *pixel = intensity;
            }
        }
    }

    return testImage;
}

// Callback function for forward scroll
void ScrollForwardCallback(vtkObject* caller, unsigned long, void* clientData, void*) {
    RendererManager* rendererManager = static_cast<RendererManager*>(clientData);
    if (rendererManager) {
        rendererManager->OnScrollForward();
    }
}

// Callback function for backward scroll
void ScrollBackwardCallback(vtkObject* caller, unsigned long, void* clientData, void*) {
    RendererManager* rendererManager = static_cast<RendererManager*>(clientData);
    if (rendererManager) {
        rendererManager->OnScrollBackward();
    }
}

void RendererManager::enableSliceInteraction() {
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = m_vtkWidget->renderWindow()->GetInteractor();
    if (!interactor) {
        interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        m_vtkWidget->renderWindow()->SetInteractor(interactor);
        interactor->Initialize();
    }

    vtkSmartPointer<ScrollInteractorStyle> interactorStyle = vtkSmartPointer<ScrollInteractorStyle>::New();
    interactor->SetInteractorStyle(interactorStyle);

    vtkSmartPointer<vtkCallbackCommand> forwardCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    forwardCallback->SetClientData(this);
    forwardCallback->SetCallback(ScrollForwardCallback);
    interactorStyle->AddObserver(vtkCommand::MouseWheelForwardEvent, forwardCallback);

    vtkSmartPointer<vtkCallbackCommand> backwardCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    backwardCallback->SetClientData(this);
    backwardCallback->SetCallback(ScrollBackwardCallback);
    interactorStyle->AddObserver(vtkCommand::MouseWheelBackwardEvent, backwardCallback);

    qDebug() << "Slice interaction style with scroll callbacks enabled.";
}

void RendererManager::disableSliceInteraction() {
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = m_vtkWidget->renderWindow()->GetInteractor();
    if (interactor) {
        interactor->SetInteractorStyle(nullptr);
        qDebug() << "Slice interaction style disabled.";
    }
}

void RendererManager::enableStyle3DInteraction() {
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = m_vtkWidget->renderWindow()->GetInteractor();
    if (!interactor) {
        interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        m_vtkWidget->renderWindow()->SetInteractor(interactor);
        interactor->Initialize();
    }

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style3D = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle(style3D);
    qDebug() << "3D interaction style enabled.";
}

void RendererManager::disableStyle3DInteraction() {
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = m_vtkWidget->renderWindow()->GetInteractor();
    if (interactor) {
        interactor->SetInteractorStyle(nullptr);
        qDebug() << "3D interaction style disabled.";
    }
}

void RendererManager::OnScrollForward() {
    if (!m_sliceMapper) return;

    m_currentSlice = (m_currentSlice + 1) > m_maxSlices ? 0 : m_currentSlice + 1;
    m_needRender = true;
    m_renderTimer.start();
}

void RendererManager::OnScrollBackward() {
    if (!m_sliceMapper) return;

    m_currentSlice = (m_currentSlice - 1) < 0 ? m_maxSlices : m_currentSlice - 1;
    m_needRender = true;
    m_renderTimer.start();
}

void RendererManager::updateRenderer() {
    if (!m_needRender || !m_sliceMapper) return;

    m_sliceMapper->SetSliceNumber(m_currentSlice);
    m_vtkWidget->renderWindow()->Render();
    m_needRender = false;
}
