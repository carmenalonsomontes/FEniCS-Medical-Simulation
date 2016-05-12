#include "workflowdata.h"

WorkflowData::WorkflowData()
{
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


void WorkflowData::clean()
{
    _CategoryList.clear();
}

bool WorkflowData::isEmpty()
{
    return _CategoryList.isEmpty();
}

