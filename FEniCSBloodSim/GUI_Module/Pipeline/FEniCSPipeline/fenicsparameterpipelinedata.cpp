#include "fenicsparameterpipelinedata.h"

FEniCSParameterPipelineData::FEniCSParameterPipelineData()
{
    _parameterName = "";
    _methodName = "";
    _description = "";
    _selectedValue  = "";
    _defaultValue  = "";
    _comboValueSelected  = "";
    _combo_fenics_value = "";

}
QString FEniCSParameterPipelineData::getParameterName()
{
    return _parameterName;
}

QString FEniCSParameterPipelineData::getMethodName()
{
    return _methodName;
}

QString FEniCSParameterPipelineData::getDescription()
{
    return _description;
}

QString FEniCSParameterPipelineData::getSelectedValue()
{
    return _selectedValue;
}

QString FEniCSParameterPipelineData::getDefaultValue()
{
    return _defaultValue;
}

QString FEniCSParameterPipelineData::getComboValueSelected()
{
    return _comboValueSelected;
}

QString FEniCSParameterPipelineData::getCombo_fenics_value()
{
    return _combo_fenics_value;
}




void FEniCSParameterPipelineData::setParameterName(QString val)
{
    _parameterName = val;
}

void FEniCSParameterPipelineData::setMethodName(QString val)
{
    _methodName = val;
}

void FEniCSParameterPipelineData::setDescription(QString val)
{
    _description = val;
}

void FEniCSParameterPipelineData::setSelectedValue(QString val)
{
    _selectedValue = val;
}

void FEniCSParameterPipelineData::setDefaultValue(QString val)
{
    _defaultValue = val;
}

void FEniCSParameterPipelineData::sgetComboValueSelected(QString val)
{
    _comboValueSelected = val;
}

void FEniCSParameterPipelineData::setCombo_fenics_value(QString val)
{
    _combo_fenics_value = val;
}


void FEniCSParameterPipelineData::clear()
{
    _parameterName.clear();
    _methodName.clear();
    _description.clear();
    _selectedValue.clear();
    _defaultValue.clear();
    _comboValueSelected.clear();
    _combo_fenics_value.clear();

}
