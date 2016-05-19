#include "categorywkfdata.h"

CategoryWkfData::CategoryWkfData()
{
    _categoryName = "";
    _iconPath = "";
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

QString CategoryWkfData::getIconPath()
{
    return _iconPath;
}

void CategoryWkfData::setIconPath(QString iconPath)
{
    _iconPath = iconPath;
}



void CategoryWkfData::clean()
{
    _categoryName.clear();
    _iconPath.clear();
    _listFunctions.clear();
}
bool CategoryWkfData::isEmpty()
{
    bool _isEmpty = true;
    if ((!_categoryName.isEmpty()) || (!_listFunctions.isEmpty()) || (_iconPath.isEmpty()))
        _isEmpty = false;
    return _isEmpty;

}
