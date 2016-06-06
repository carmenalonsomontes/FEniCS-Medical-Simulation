#ifndef IMAGEWORKFLOW_H
#define IMAGEWORKFLOW_H
#include "genericworkflowdialog.h"

#include "GUI_Module/UIHelpers/workflowtablehelper.h"
//#include "GUI_Module/DragItem/dragitem.h"
#include "GUI_Module/Data/WorkflowData/workflowdata.h"
#include "GUI_Module/Data/WorkflowData/categorywkfdata.h"
#include "GUI_Module/Data/WorkflowData/imagingwkffunctiondata.h"

class ImageWorkflow: public GenericWorkflowDialog
{
    Q_OBJECT
public:
    ImageWorkflow();

    void setImagePath(QString imPath);


private:

    void loadSpecificMethods();
    void loadInitialMenu();
    void fillMenuTable(CategoryWkfData _catObj);

    void fillWorkflowTabInfo(QList<CategoryWkfData> catList);
    void createCategoryTabs(int noTabs,QList<CategoryWkfData> catList);


};

#endif // IMAGEWORKFLOW_H
