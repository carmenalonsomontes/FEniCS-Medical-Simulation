#include "functionfenicsdata.h"

FunctionFEniCSData::FunctionFEniCSData()
{
    _functionName = "";
    _fenicsFunctionName = "";
    _description = "";
 }
QString FunctionFEniCSData::getFunctionName()
{
    return _functionName;
}

QString FunctionFEniCSData::getFenicsFunctionName()
{
    return _fenicsFunctionName;
}
QString FunctionFEniCSData::getDescription()
{
    return _description;
}


void FunctionFEniCSData::setFunctionName(QString functionName)
{
    _functionName = functionName;
}
void FunctionFEniCSData::setFenicsFunctionName(QString fenicsName)
{
    _fenicsFunctionName = fenicsName;
}
void FunctionFEniCSData::setDescription(QString newDesc)
{
    _description = newDesc;
}

QList<ParameterFEniCSData> FunctionFEniCSData::getParameterList()
{
    return _parameterList;
}

void FunctionFEniCSData::setParameterList( QList<ParameterFEniCSData> list)
{
    _parameterList = list;
}
bool FunctionFEniCSData::hasParameters()
{
    if (_parameterList.size() > 0)
        return true;
    else
        return false;
}


void FunctionFEniCSData::clear()
{
    _functionName.clear();
    _fenicsFunctionName.clear();
    _description.clear();
    _parameterList.clear();
}

bool FunctionFEniCSData::isEmpty()
{
    if ((_functionName.isEmpty()) && (_fenicsFunctionName.isEmpty()) && (_description.isEmpty()))
        return true;
    else
        return false;
}
