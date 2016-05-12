#ifndef CATEGORYWKFDATA_H
#define CATEGORYWKFDATA_H

#include <QList>
#include <QString>
#include "imagingwkffunctiondata.h"


class CategoryWkfData
{
public:
    CategoryWkfData();

    QString getCategoryName();
    QList<ImagingWkfFunctionData> getListFunctions();

    void setCategoryName(QString newCategory);
    void setListFunctions(QList<ImagingWkfFunctionData> newFunctionList);
    void addFunction(ImagingWkfFunctionData newFunction);

    void clean();
    bool isEmpty();

private:
    QString _categoryName;
    QList<ImagingWkfFunctionData> _listFunctions;
};

#endif // CATEGORYWKFDATA_H
