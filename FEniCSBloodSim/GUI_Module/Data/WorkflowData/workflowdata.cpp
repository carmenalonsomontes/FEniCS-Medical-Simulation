#include "workflowdata.h"

WorkflowData::WorkflowData()
{
    _imPath = "";
}



QList<CategoryWkfData> WorkflowData::getCategoryList()
{
    return _CategoryList;

}

void WorkflowData::setCategoryList(QList<CategoryWkfData> newList)
{
    _CategoryList = newList;
}

void WorkflowData::addCategory(CategoryWkfData newCategory)
{
    if (!newCategory.isEmpty())
        _CategoryList.append(newCategory);
}



void WorkflowData::setImagePath(QString newImPath)
{
    _imPath = newImPath;
}

QString WorkflowData::getImagePath()
{
    return _imPath;
}

void WorkflowData::clean()
{
    _CategoryList.clear();
    _imPath.clear();
}

bool WorkflowData::isEmpty()
{
    return _CategoryList.isEmpty();
}

