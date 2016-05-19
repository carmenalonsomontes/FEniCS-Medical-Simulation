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

    QString getIconPath();
    void setIconPath(QString iconPath);

    void clean();
    bool isEmpty();

private:
    QString _categoryName;
    QString _iconPath;
    QList<ImagingWkfFunctionData> _listFunctions;
};

#endif // CATEGORYWKFDATA_H
