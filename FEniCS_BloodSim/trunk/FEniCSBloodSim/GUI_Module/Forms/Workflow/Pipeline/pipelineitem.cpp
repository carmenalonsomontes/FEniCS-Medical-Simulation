#include "pipelineitem.h"

PipelineItem::PipelineItem()
{

    _iconPath = "";
    _description = "";
    _categoryName = "";
    _positionPipeline = 0;

    _functionName = "";
    _functionClassName = "";
    _functionDescription = "";

    _image3D = ImageType3D::New();
    _image2D = ImageType2D::New();
}



QString PipelineItem::getIconPath()
{
    return _iconPath;
}

QString PipelineItem::getDescription()
{
    return _description;
}

int PipelineItem::getPositionPipeline()
{
    return _positionPipeline;
}


void PipelineItem::setIconPath(QString newPath)
{
    _iconPath = newPath;
}

void PipelineItem::setDescription(QString newDesc)
{
    _description = newDesc;
}

void PipelineItem::setPositionPipeline(int newPos)
{
    _positionPipeline = newPos;
}


QString PipelineItem::getCategoryName()
{
    return _categoryName;
}

void PipelineItem::setCategoryName(QString newCatName)
{
    _categoryName = newCatName;
}


QList<ConfigurationPipelineItem> PipelineItem::getConfigurationItemList()
{
    return _configurationItemList;
}

void  PipelineItem::setConfigurationItemList(QList<ConfigurationPipelineItem> list)
{
    _configurationItemList = list;
}


QString PipelineItem::getFunctionName()
{
    return _functionName;
}
QString PipelineItem::getFunctionClassName()
{
    return _functionClassName;
}
QString PipelineItem::getFunctionDescription()
{
    return _functionDescription;
}

void PipelineItem::setFunctionName(QString name)
{
    _functionName = name;
}
void PipelineItem::setFunctionClassName(QString className)
{
    _functionClassName = className;
}
void PipelineItem::setFunctionDescription(QString  desc)
{
    _functionDescription = desc;
}


void  PipelineItem::updateConfiguredItemValue(QString _itemValue, int pos)
{
    ConfigurationPipelineItem _item = _configurationItemList.at(pos);
    _item.setCurrentValue(_itemValue);
    _configurationItemList.replace(pos,_item);
}


ImageType3D::Pointer PipelineItem::getImage3D()
{
    return _image3D;
}

void PipelineItem::setImage3D( ImageType3D::Pointer _image)
{
    _image3D = _image;
}

ImageType2D::Pointer PipelineItem::getImage2D()
{
    return _image2D;
}

void PipelineItem::setImage2D( ImageType2D::Pointer _image)
{
    _image2D = _image;
}




bool PipelineItem::isEmpty()
{
   bool _isEmpty = true;
   if ((!_iconPath.isEmpty()) || (!_description.isEmpty())
           || (!_categoryName.isEmpty()))
           _isEmpty = false;

   return _isEmpty;
}

void PipelineItem::clear()
{
    _iconPath.clear();
    _description.clear();
    _categoryName.clear();
    _configurationItemList.clear();
    _positionPipeline = 0;


    _functionName.clear();
    _functionClassName.clear();
    _functionDescription.clear();

    _image2D = NULL;
    _image3D = NULL;
}
