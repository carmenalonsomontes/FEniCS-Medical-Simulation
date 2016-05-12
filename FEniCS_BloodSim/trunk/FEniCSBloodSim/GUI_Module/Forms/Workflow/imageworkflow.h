#ifndef IMAGEWORKFLOW_H
#define IMAGEWORKFLOW_H
#include "genericworkflowdialog.h"

#include "GUI_Module/UIHelpers/workflowtablehelper.h"
#include "GUI_Module/DragItem/dragitem.h"
class ImageWorkflow: public GenericWorkflowDialog
{
public:
    ImageWorkflow();
    void loadDragableMethods();

private:

   //void loadIcons(int row);

};

#endif // IMAGEWORKFLOW_H
