#include "ScrollInteractorStyle.h"
#include <vtkObjectFactory.h>

vtkStandardNewMacro(ScrollInteractorStyle);

void ScrollInteractorStyle::OnMouseWheelForward() {
    // Trigger custom forward scroll command
    this->InvokeEvent(vtkCommand::MouseWheelForwardEvent);  // Invoke the VTK forward event
    vtkInteractorStyleImage::OnMouseWheelForward();  // Call the base class function
}

void ScrollInteractorStyle::OnMouseWheelBackward() {
    // Trigger custom backward scroll command
    this->InvokeEvent(vtkCommand::MouseWheelBackwardEvent);  // Invoke the VTK backward event
    vtkInteractorStyleImage::OnMouseWheelBackward();  // Call the base class function
}
