#include "configurationpipelineitem.h"

ConfigurationPipelineItem::ConfigurationPipelineItem()
{
    _optionDefaultValue = "";
    _optionType = "";
    _optionName = "";
    _cValue = "";
    _methodName = "";
}


QString ConfigurationPipelineItem::getOptionName()
{
    return _optionName;
}

QString ConfigurationPipelineItem::getOptionType()
{
    return _optionType;
}

QString ConfigurationPipelineItem::getOptionDefaultValue()
{
    return _optionDefaultValue;
}

void ConfigurationPipelineItem::setOptionName(QString newName)
{
    _optionName = newName;
}

void ConfigurationPipelineItem::setOptionType(QString newName)
{
    _optionType = newName;
}

void ConfigurationPipelineItem::setOptionDefaultValue(QString newName)
{
    _optionDefaultValue = newName;
}

QString ConfigurationPipelineItem::getCurrentValue()
{
    return _cValue;
}

void ConfigurationPipelineItem::setCurrentValue(QString newValue)
{
    _cValue = newValue;
}


QString ConfigurationPipelineItem::getMethodName()
{
    return _methodName;
}

void ConfigurationPipelineItem::setMethodName(QString newName)
{
    _methodName = newName;
}

bool ConfigurationPipelineItem::isEmpty()
{
    bool _isEmpty = true;
    if( (!_optionDefaultValue.isEmpty()) || (_optionName.isEmpty()) )
        _isEmpty = false;

    return _isEmpty;
}

void ConfigurationPipelineItem::clear()
{
    _optionDefaultValue.clear();
    _optionName.clear();
    _optionType.clear();
    _methodName.clear();
    _cValue.clear();
}
