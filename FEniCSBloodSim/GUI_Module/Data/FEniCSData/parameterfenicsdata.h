#ifndef PARAMETERFENICSDATA_H
#define PARAMETERFENICSDATA_H

#include <QString>
#include <QList>

#include "combovaluesfenicsdata.h"

class ParameterFEniCSData
{
public:
    ParameterFEniCSData();


    QString getName();
    QString getMethodName();
    QString getDescription();


    void setName(QString newName);
    void setMethodName(QString newMethodName);
    void setDescription(QString newDesc);

    QString getDefaultValue();
    bool isOptional();

    void setDefaultValue(QString val);
    void setOptional(bool val);

    QList<ComboValuesFEniCSData> getComboValuesList();
    void setComboValuesList(QList<ComboValuesFEniCSData> list);
    bool hasComboValues();

    QString getInput();
    void setInput(QString val);

    void clear();
    bool isEmpty();


private:
    QString _name;
    QString _methodName;
    QString _description;
    QString _defaultValue;
    QString _input;
    bool _isOptional;
    QList<ComboValuesFEniCSData> _comboValues;

};

#endif // PARAMETERFENICSDATA_H
