#ifndef FENICSPIPELINEDATA_H
#define FENICSPIPELINEDATA_H

#include <QList>
#include <QString>
#include <QStringList>

#include "fenicsparameterpipelinedata.h"

class FEniCSPipelineData
{
public:
    FEniCSPipelineData();

    QString getCategoryName();
    QString getKeyInput();
    QString getFunctionName();
    QString getFunctionFenicsName();
    QString getFunctionDesc();
    QList<FEniCSParameterPipelineData> getParameterPipelineItems();


    void setCategoryName(QString val);
    void setKeyInput(QString val);
    void setFunctionName(QString val);
    void setFunctionFenicsName(QString val);
    void setFunctionDesc(QString val);
    void setParameterPipelineItems( QList<FEniCSParameterPipelineData> list);

    void updateParameterItemValue(QString _itemValue, int pos);
    void clear();


private:

    QString _categoryName;
    QString _keyInput;
    QString _functionName;
    QString _functionFenicsName;
    QString _functionDesc;
    QList<FEniCSParameterPipelineData> _parameterPipelineItems;

};

#endif // FENICSPIPELINEDATA_H
