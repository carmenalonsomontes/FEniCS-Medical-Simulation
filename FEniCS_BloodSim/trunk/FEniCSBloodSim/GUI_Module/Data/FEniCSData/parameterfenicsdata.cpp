#include "parameterfenicsdata.h"

ParameterFEniCSData::ParameterFEniCSData()
{
    _name = "";
    _methodName = "";
    _description = "";
    _defaultValue = "";
    _isOptional = false;
    _input = "";
}


QString ParameterFEniCSData::getName()
{
    return _name;
}
QString ParameterFEniCSData::getMethodName()
{
    return _methodName;
}
QString ParameterFEniCSData::getDescription()
{
    return _description;
}


void ParameterFEniCSData::setName(QString newName)
{
    _name = newName;
}
void ParameterFEniCSData::setMethodName(QString newMethodName)
{
    _methodName = newMethodName;
}
void ParameterFEniCSData::setDescription(QString newDesc)
{
    _description = newDesc;
}


QString ParameterFEniCSData::getDefaultValue()
{
    return _defaultValue;
}

bool ParameterFEniCSData::isOptional()
{
    return _isOptional;
}

void ParameterFEniCSData::setDefaultValue(QString val)
{
    _defaultValue = val;
}
void ParameterFEniCSData::setOptional(bool val)
{
    _isOptional = val;
}


QList<ComboValuesFEniCSData> ParameterFEniCSData::getComboValuesList()
{
    return _comboValues;

}

void ParameterFEniCSData::setComboValuesList(QList<ComboValuesFEniCSData> list)
{
    _comboValues = list;
}


QString ParameterFEniCSData::getInput()
{
   return _input;
}

void ParameterFEniCSData::setInput(QString val)
{
    _input = val;
}


bool  ParameterFEniCSData::hasComboValues()
{
    if (_comboValues.size()>0)
        return true;
    else
        return false;
}

void ParameterFEniCSData::clear()
{
    _name.clear();
    _methodName.clear();
    _description.clear();
    _defaultValue.clear();
    _isOptional = false;
    _comboValues.clear();
    _input.clear();
}
bool ParameterFEniCSData::isEmpty()
{
    if ((_name.isEmpty()) && (_methodName.isEmpty()) && (_description.isEmpty()))
        return true;
    else
        return false;
}
