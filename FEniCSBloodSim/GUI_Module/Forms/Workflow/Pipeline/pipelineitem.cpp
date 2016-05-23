#include "pipelineitem.h"

PipelineItem::PipelineItem()
{

    _iconPath = "";
    _description = "";
    _positionPipeline = 0;
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

bool PipelineItem::isEmpty()
{
   bool _isEmpty = true;
   if ((!_iconPath.isEmpty()) || (!_description.isEmpty()))
           _isEmpty = false;

   return _isEmpty;
}


void PipelineItem::clear()
{
    _iconPath.clear();
    _description.clear();
    _positionPipeline = 0;
}
