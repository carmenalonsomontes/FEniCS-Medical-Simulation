#include "categoryfenicsdata.h"

CategoryFEniCSData::CategoryFEniCSData()
{
    _categoryName = "";
    _keyInput = "";
}
QString CategoryFEniCSData::getCategoryName()
{
    return _categoryName;
}

void CategoryFEniCSData::setCategoryName(QString newCategory)
{
    _categoryName = newCategory;
}

QString CategoryFEniCSData::getKeyInput()
{
    return _keyInput;
}

void CategoryFEniCSData::setKeyInput(QString keyVal)
{
    _keyInput = keyVal;
}


QList<FunctionFEniCSData> CategoryFEniCSData::getListFunctions()
{
    return _listFunctions;
}

void CategoryFEniCSData::setListFunctions(QList<FunctionFEniCSData> list)
{
    _listFunctions = list;
}

bool CategoryFEniCSData::hasFunctions()
{
    if (_listFunctions.size() > 0)
        return true;
    else
        return false;
}


void CategoryFEniCSData::clean()
{
    _categoryName.clear();
    _keyInput.clear();
}

bool CategoryFEniCSData::isEmpty()
{
    if (_categoryName.isEmpty())
        return true;
    else
        return false;
}
