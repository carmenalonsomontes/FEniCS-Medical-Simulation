#include "combovaluesfenicsdata.h"

ComboValuesFEniCSData::ComboValuesFEniCSData()
{
    _name = "";
    _fenicsValue = "";
}


QString ComboValuesFEniCSData::getName()
{
    return _name;
}

QString ComboValuesFEniCSData::getFenicsValue()
{
    return _fenicsValue;
}

void ComboValuesFEniCSData::setName(QString val)
{
    _name = val;
}
void ComboValuesFEniCSData::setFenicsValue(QString val)
{
    _fenicsValue = val;

}

void ComboValuesFEniCSData::clear()
{
    _name.clear();
    _fenicsValue.clear();
}
bool ComboValuesFEniCSData::isEmpty()
{
    if ((_name.isEmpty()) && (_fenicsValue.isEmpty()))
        return true;
    else
        return false;
}
