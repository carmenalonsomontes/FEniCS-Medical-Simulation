#include "fenicsxmldata.h"

FEniCSXMLData::FEniCSXMLData()
{
}



QList<CategoryFEniCSData> FEniCSXMLData::getCategoryList()
{
    return _categoryList;

}

void FEniCSXMLData::setCategoryList(QList<CategoryFEniCSData> newList)
{
    _categoryList = newList;
}
void FEniCSXMLData::addCategory(CategoryFEniCSData newCategory)
{
    if (!newCategory.isEmpty())
        _categoryList.append(newCategory);
}

void FEniCSXMLData::clean()
{
    _categoryList.clear();
}
bool FEniCSXMLData::isEmpty()
{
    return _categoryList.isEmpty();
}
