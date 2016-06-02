#include "imagingwkffunctiondata.h"

ImagingWkfFunctionData::ImagingWkfFunctionData()
{
    _name = "";
    _className = "";
    _description = "";
   // _parameters = "";

}



QString ImagingWkfFunctionData::getName()
{
    return _name;
}

QString ImagingWkfFunctionData::getClassName()
{
    return _className;
}
QString ImagingWkfFunctionData::getDescription()
{
    return _description;
}
/*
QString ImagingWkfFunctionData::getParameters()
{
    return _parameters;
}*/


void ImagingWkfFunctionData::setName(QString newName)
{
    _name = newName;
}
void ImagingWkfFunctionData::setClassName(QString newClassname)
{
    _className = newClassname;
}
void ImagingWkfFunctionData::setDescription(QString newDescription)
{
    _description = newDescription;
}
/*void ImagingWkfFunctionData::setParameters(QString newParameters)
{
    _parameters = newParameters;
}
*/


QStringList ImagingWkfFunctionData::getParameterMethodName()
{
    return _parameterMethodName;
}

QStringList ImagingWkfFunctionData::getParametersName()
{
    return _parametersName;
}

QStringList ImagingWkfFunctionData::getParameterType()
{
    return _parameterType;
}

void ImagingWkfFunctionData::setParameterMethodName(QStringList list)
{
    _parameterMethodName = list;
}

void ImagingWkfFunctionData::setParametersName(QStringList list)
{
    _parametersName = list;
}

void ImagingWkfFunctionData::setParameterType(QStringList list)
{
    _parameterType = list;
}

QStringList ImagingWkfFunctionData::getDefaultValue()
{
    return _defaultValue;
}

void  ImagingWkfFunctionData::setDefaultValue(QStringList newList)
{
    _defaultValue = newList;
}


void ImagingWkfFunctionData::clear()
{
    _name.clear();
    _className.clear();
    _description.clear();
   // _parameters.clear();
    _parameterMethodName.clear();
    _parametersName.clear();
    _parameterType.clear();
    _defaultValue.clear();
}
bool ImagingWkfFunctionData::isEmpty()
{
    bool _isEmpty = true;
    if ((!_name.isEmpty()) || (!_className.isEmpty()) || (!_description.isEmpty()) )
        _isEmpty = false;
    return _isEmpty;
}
