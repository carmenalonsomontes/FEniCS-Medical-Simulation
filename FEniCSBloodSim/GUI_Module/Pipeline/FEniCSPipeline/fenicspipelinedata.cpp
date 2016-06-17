#include "fenicspipelinedata.h"

FEniCSPipelineData::FEniCSPipelineData()
{
    _categoryName = "";
    _keyInput = "";
    _functionName = "";
    _functionFenicsName = "";
    _functionDesc =  "";
}


QString FEniCSPipelineData::getCategoryName()
{
    return _categoryName;
}
QString FEniCSPipelineData::getKeyInput()
{
    return _keyInput;
}
QString FEniCSPipelineData::getFunctionName()
{
    return _functionName;
}
QString FEniCSPipelineData::getFunctionFenicsName()
{
    return _functionFenicsName;
}
QString FEniCSPipelineData::getFunctionDesc()
{
    return _functionDesc;
}
QList<FEniCSParameterPipelineData> FEniCSPipelineData::getParameterPipelineItems()
{
    return _parameterPipelineItems;
}


void FEniCSPipelineData::setCategoryName(QString val)
{
    _categoryName = val;
}
void FEniCSPipelineData::setKeyInput(QString val)
{
    _keyInput = val;
}
void FEniCSPipelineData::setFunctionName(QString val)
{
    _functionName = val;
}
void FEniCSPipelineData::setFunctionFenicsName(QString val)
{
    _functionFenicsName = val;
}
void FEniCSPipelineData::setFunctionDesc(QString val)
{
    _functionDesc = val;
}
void FEniCSPipelineData::setParameterPipelineItems( QList<FEniCSParameterPipelineData> list)
{
    _parameterPipelineItems = list;
}


void  FEniCSPipelineData::updateParameterItemValue(QString _itemValue, int pos)
{
    FEniCSParameterPipelineData _item = _parameterPipelineItems.at(pos);
    _item.setSelectedValue(_itemValue);
    _parameterPipelineItems.replace(pos,_item);
}
void FEniCSPipelineData::clear()
{
    _categoryName.clear();
    _keyInput.clear();
    _functionName.clear();
    _functionFenicsName.clear();
    _functionDesc.clear();
    _parameterPipelineItems.clear();
}
