#ifndef WORKFLOWDATA_H
#define WORKFLOWDATA_H
#include <QString>
#include <QList>

#include "GUI_Module/Data/WorkflowData/imagingwkffunctiondata.h"
#include "GUI_Module/Data/WorkflowData/categorywkfdata.h"

class WorkflowData
{
public:
    WorkflowData();

    QList<CategoryWkfData> getCategoryList();
    void setCategoryList(QList<CategoryWkfData> newList);
    void addCategory(CategoryWkfData newCategory);

    void clean();
    bool isEmpty();

private:

    QList<CategoryWkfData> _CategoryList;

};

#endif // WORKFLOWDATA_H
