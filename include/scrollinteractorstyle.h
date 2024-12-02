#ifndef SCROLLINTERACTORSTYLE_H
#define SCROLLINTERACTORSTYLE_H

#include <vtkInteractorStyleImage.h>
#include <vtkSmartPointer.h>
#include <vtkCommand.h>

class ScrollInteractorStyle : public vtkInteractorStyleImage {
public:
    static ScrollInteractorStyle* New();
    vtkTypeMacro(ScrollInteractorStyle, vtkInteractorStyleImage);

    // Override scroll events
    void OnMouseWheelForward() override;
    void OnMouseWheelBackward() override;
};

#endif // SCROLLINTERACTORSTYLE_H
