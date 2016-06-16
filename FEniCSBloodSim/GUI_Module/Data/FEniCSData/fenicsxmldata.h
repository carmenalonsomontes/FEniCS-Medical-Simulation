#ifndef FENICSXMLDATA_H
#define FENICSXMLDATA_H

#include <QString>
#include <QList>


#include "GUI_Module/Data/FEniCSData/categoryfenicsdata.h"


class FEniCSXMLData
{
public:
    FEniCSXMLData();

    QList<CategoryFEniCSData> getCategoryList();
    void setCategoryList(QList<CategoryFEniCSData> newList);
    void addCategory(CategoryFEniCSData newCategory);

    void clean();
    bool isEmpty();


private:

    QList<CategoryFEniCSData> _categoryList;


};

#endif // FENICSXMLDATA_H
