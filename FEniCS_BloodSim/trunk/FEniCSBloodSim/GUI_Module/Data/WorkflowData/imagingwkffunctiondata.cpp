#include "imagingwkffunctiondata.h"

ImagingWkfFunctionData::ImagingWkfFunctionData()
{
    _name = "";
    _className = "";
    _description = "";
    _parameters = "";

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
QString ImagingWkfFunctionData::getParameters()
{
    return _parameters;
}


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
void ImagingWkfFunctionData::setParameters(QString newParameters)
{
    _parameters = newParameters;
}

void ImagingWkfFunctionData::clear()
{
    _name.clear();
    _className.clear();
    _description.clear();
    _parameters.clear();
}
bool ImagingWkfFunctionData::isEmpty()
{
    bool _isEmpty = true;
    if ((!_name.isEmpty()) || (!_className.isEmpty()) || (!_description.isEmpty())
            || (!_parameters.isEmpty()))
        _isEmpty = false;
    return _isEmpty;
}
