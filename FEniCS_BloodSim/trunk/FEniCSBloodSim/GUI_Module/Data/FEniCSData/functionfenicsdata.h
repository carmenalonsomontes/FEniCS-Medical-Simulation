#ifndef FUNCTIONFENICSDATA_H
#define FUNCTIONFENICSDATA_H

#include <QString>
#include <QList>

#include "parameterfenicsdata.h"
class FunctionFEniCSData
{
public:
    FunctionFEniCSData();

    QString getFunctionName();
    QString getFenicsFunctionName();
    QString getDescription();


    void setFunctionName(QString functionName);
    void setFenicsFunctionName(QString fenicsName);
    void setDescription(QString newDesc);

    QList<ParameterFEniCSData> getParameterList();
    void setParameterList( QList<ParameterFEniCSData> list);
    bool hasParameters();


    void clear();
    bool isEmpty();

private:

    QString _functionName;
    QString _fenicsFunctionName;
    QString _description;

    QList<ParameterFEniCSData> _parameterList;


};

#endif // FUNCTIONFENICSDATA_H
