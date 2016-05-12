#include "categorywkfdata.h"

CategoryWkfData::CategoryWkfData()
{
    _categoryName = "";
}

QString CategoryWkfData::getCategoryName()
{
    return _categoryName;
}

QList<ImagingWkfFunctionData> CategoryWkfData::getListFunctions()
{
   return _listFunctions;
}

void CategoryWkfData::setCategoryName(QString newCategory)
{
    _categoryName = newCategory;
}

void CategoryWkfData::setListFunctions(QList<ImagingWkfFunctionData> newFunctionList)
{
    _listFunctions = newFunctionList;
}

void CategoryWkfData::addFunction(ImagingWkfFunctionData newFunction)
{
    if (newFunction.isEmpty())
    _listFunctions.append(newFunction);
}


void CategoryWkfData::clean()
{
    _categoryName.clear();
    _listFunctions.clear();
}
bool CategoryWkfData::isEmpty()
{
    bool _isEmpty = true;
    if ((!_categoryName.isEmpty()) || (!_listFunctions.isEmpty()))
        _isEmpty = false;
    return _isEmpty;

}
