#ifndef CATEGORYFENICSDATA_H
#define CATEGORYFENICSDATA_H
#include <QList>
#include <QString>
#include "functionfenicsdata.h"


class CategoryFEniCSData
{
public:
    CategoryFEniCSData();

    QString getCategoryName();
    void setCategoryName(QString newCategory);
    QString getKeyInput();
    void setKeyInput(QString keyVal);

    QList<FunctionFEniCSData> getListFunctions();
    void setListFunctions(QList<FunctionFEniCSData> list);
    bool hasFunctions();

    void clean();
    bool isEmpty();

private:
    QString _categoryName;
    QString _keyInput;
    QList<FunctionFEniCSData> _listFunctions;
};

#endif // CATEGORYFENICSDATA_H
