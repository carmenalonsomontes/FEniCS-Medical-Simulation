#include "xmlprojectdata.h"


#include <QDir>
#include "../Defines/Menu/MenuDefines.h"

XMLProjectData::XMLProjectData()
{
}

bool XMLProjectData::saveProjectDataFile(FBS_ProjectData * _projectData)
{
    QString _filePath = QDir(_projectData->getProjectPath()).filePath(_projectData->getProjectName()) + "." + FBS_FILE_PROJECT_EXT ;

    QFile _file(_filePath);
    bool _success = false;

    if (_file.exists())
        _file.remove();
    _success= createNewFile(&_file,_projectData);

    return _success;
}


void XMLProjectData::readProjectDataFile(FBS_ProjectData * _projectData, QString path)
{
    bool _fileRead = true;
    QFile _file(path);
    _fileRead = _file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (_fileRead)
    {
        QXmlStreamReader xmlReader(&_file);
        readProjectData(&xmlReader,_projectData);
    }
    _file.close();
}


void XMLProjectData::readProjectData( QXmlStreamReader * xmlReader, FBS_ProjectData * _projectData)
{

    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()))
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if (token == QXmlStreamReader::StartDocument) continue;
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), FBS_PROJECT_TAG) == StrEqual) )
        {
                QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                if (_elementAtt.hasAttribute(FBS_PROJECT_PATH_ATT))
                    _projectData->setProjectPath(_elementAtt.value(FBS_PROJECT_PATH_ATT).toString());
                if (_elementAtt.hasAttribute(FBS_PROJECT_NAME_ATT))
                     _projectData->setProjectName(_elementAtt.value(FBS_PROJECT_NAME_ATT).toString());
        }
        if ((token == QXmlStreamReader::StartElement) &&
            (QString::compare(xmlReader->name().toString(), FBS_IMAGE_TAG) == StrEqual) )
        {
                QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                if (_elementAtt.hasAttribute(FBS_IMAGE_PATH_ATT))
                    _projectData->setImPath(_elementAtt.value(FBS_IMAGE_PATH_ATT).toString());
                if (_elementAtt.hasAttribute(FBS_IMAGE_NAME_ATT))
                     _projectData->setImName(_elementAtt.value(FBS_IMAGE_NAME_ATT).toString());
                if (_elementAtt.hasAttribute(FBS_IMAGE_PREFIX_NAME_ATT))
                     _projectData->setImPrefixSeries(_elementAtt.value(FBS_IMAGE_PREFIX_NAME_ATT).toString());
        }
        // This tag contains the PIPELINE ITEMS - IMAGE PROCESSING
        if ((token == QXmlStreamReader::StartElement) &&
            (QString::compare(xmlReader->name().toString(), FBS_IMAGE_PIPELINE_TAG) == StrEqual) )
            readPipelineFunctions(xmlReader,_projectData );

    }

}


void  XMLProjectData::readPipelineFunctions(QXmlStreamReader * xmlReader, FBS_ProjectData * _projectData )
{
    bool _endPipeline = false;
    QList<PipelineItem> _pipelineItems;

     while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!_endPipeline))
     {
         QXmlStreamReader::TokenType token = xmlReader->readNext();


         if ((token == QXmlStreamReader::StartElement) &&
                 (QString::compare(xmlReader->name().toString(), FBS_IMAGE_PIPELINE_FUNCTION_TAG) == StrEqual) )
         {
             QXmlStreamAttributes _elementAtt = xmlReader->attributes();
             PipelineItem _item;

             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_FUNCTION_NAME_ATT))
                 _item.setFunctionName(_elementAtt.value(FBS_IMAGE_PIPELINE_FUNCTION_NAME_ATT).toString());

             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_FUNCTION_CLASSNAME_ATT))
                  _item.setFunctionClassName(_elementAtt.value(FBS_IMAGE_PIPELINE_FUNCTION_CLASSNAME_ATT).toString());

             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_FUNCTION_DESCRIPTION_ATT))
                  _item.setFunctionDescription(_elementAtt.value(FBS_IMAGE_PIPELINE_FUNCTION_DESCRIPTION_ATT).toString());

             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_FUNCTION_POSITION_PIPELINE_ATT))
                  _item.setPositionPipeline(_elementAtt.value(FBS_IMAGE_PIPELINE_FUNCTION_POSITION_PIPELINE_ATT).toString().toInt());

             // Add Category
             readPipelineCategory(xmlReader, & _item);


             // Add Parameters
             readParameters(xmlReader,& _item);


             _pipelineItems.append(_item);
         }
         if ((token == QXmlStreamReader::EndElement) &&
                 (QString::compare(xmlReader->name().toString(), FBS_IMAGE_PIPELINE_TAG) == StrEqual) )
             _endPipeline = true;

     }
     _projectData->setListPipelineItems(_pipelineItems);
}


void XMLProjectData::readPipelineCategory(QXmlStreamReader * xmlReader, PipelineItem * _item )
{

    bool _foundCategory = false;


    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!_foundCategory))
     {
         QXmlStreamReader::TokenType token = xmlReader->readNext();


         if ((token == QXmlStreamReader::StartElement) &&
                 (QString::compare(xmlReader->name().toString(), FBS_IMAGE_PIPELINE_CATEGORY_TAG) == StrEqual) )
         {
             QXmlStreamAttributes _elementAtt = xmlReader->attributes();


             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_CATEGORY_NAME_ATT))
                 _item->setCategoryName(_elementAtt.value(FBS_IMAGE_PIPELINE_CATEGORY_NAME_ATT).toString());

             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_CATEGORY_ICONPATH_ATT))
                 _item->setIconPath(_elementAtt.value(FBS_IMAGE_PIPELINE_CATEGORY_ICONPATH_ATT).toString());

             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_CATEGORY_DESCRIPTION_ATT))
                 _item->setDescription(_elementAtt.value(FBS_IMAGE_PIPELINE_CATEGORY_DESCRIPTION_ATT).toString());

             _foundCategory = true;
         }
    }

}

void XMLProjectData::readParameters(QXmlStreamReader * xmlReader, PipelineItem * _item )
{
    bool _foundParameters = false;
    QList<ConfigurationPipelineItem> _listParameters;


    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!_foundParameters))
     {
         QXmlStreamReader::TokenType token = xmlReader->readNext();


         if ((token == QXmlStreamReader::StartElement) &&
                 (QString::compare(xmlReader->name().toString(), FBS_IMAGE_PIPELINE_PARAMETER_TAG) == StrEqual) )
         {
             QXmlStreamAttributes _elementAtt = xmlReader->attributes();
             ConfigurationPipelineItem _parameter;


             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_PARAMETER_NAME_ATT))
                 _parameter.setOptionName(_elementAtt.value(FBS_IMAGE_PIPELINE_PARAMETER_NAME_ATT).toString());

             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_PARAMETER_TYPE_ATT))
                 _parameter.setOptionType(_elementAtt.value(FBS_IMAGE_PIPELINE_PARAMETER_TYPE_ATT).toString());

             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_PARAMETER_DEFAULT_VALUE_ATT))
                 _parameter.setOptionDefaultValue(_elementAtt.value(FBS_IMAGE_PIPELINE_PARAMETER_DEFAULT_VALUE_ATT).toString());

             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_PARAMETER_METHOD_NAME_ATT))
                 _parameter.setMethodName(_elementAtt.value(FBS_IMAGE_PIPELINE_PARAMETER_METHOD_NAME_ATT).toString());

             if (_elementAtt.hasAttribute(FBS_IMAGE_PIPELINE_PARAMETER_CURRENT_VALUE_ATT))
                 _parameter.setCurrentValue(_elementAtt.value(FBS_IMAGE_PIPELINE_PARAMETER_CURRENT_VALUE_ATT).toString());

            _listParameters.append(_parameter);

         }
         if ((token == QXmlStreamReader::EndElement) &&
                 (QString::compare(xmlReader->name().toString(), FBS_IMAGE_PIPELINE_PARAMETER_LIST_TAG) == StrEqual) )
             _foundParameters = true;
    }
    _item->setConfigurationItemList(_listParameters);

}



bool XMLProjectData::createNewFile(QFile * _file, FBS_ProjectData * _projectData)
{
   bool _success = false;
   _success = _file->open(QIODevice::ReadWrite);
   if (_success)
   {

       QXmlStreamWriter xmlWriter(_file);
       xmlWriter.setAutoFormatting(true);
       xmlWriter.writeStartDocument();

       xmlWriter.writeStartElement(FBS_GUI_TAG);

       writeProjectData(&xmlWriter,_projectData->getProjectPath(),_projectData->getProjectName());
       writeImageData(&xmlWriter,_projectData->getImPath(),_projectData->getImName(),_projectData->getImPrefixSeries());
       writePipeline(&xmlWriter,_projectData->getListPipelineItems());

       // Closing the root and the document
       xmlWriter.writeEndElement();
       xmlWriter.writeEndDocument();
       _file->close();
   }

   return _success;
}

void XMLProjectData::writePipeline(QXmlStreamWriter * xmlWriter, QList<PipelineItem> _pipeline)
{
        xmlWriter->writeStartElement(FBS_IMAGE_PIPELINE_TAG);

    for (int i= 0; i< _pipeline.size();i++)
        writePipelineFunction(xmlWriter,_pipeline.at(i));

    xmlWriter->writeEndElement();
}

void XMLProjectData::writePipelineFunction(QXmlStreamWriter * xmlWriter, PipelineItem _pipeline)
{
    xmlWriter->writeStartElement(FBS_IMAGE_PIPELINE_FUNCTION_TAG);
    xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_FUNCTION_NAME_ATT,_pipeline.getFunctionName());
    xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_FUNCTION_CLASSNAME_ATT,_pipeline.getFunctionClassName());
    xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_FUNCTION_DESCRIPTION_ATT,_pipeline.getFunctionDescription());
    xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_FUNCTION_POSITION_PIPELINE_ATT,QString::number(_pipeline.getPositionPipeline()));

    writeCategory(xmlWriter,_pipeline);
    writeParametersList(xmlWriter,_pipeline);

    xmlWriter->writeEndElement();
}

void XMLProjectData::writeCategory(QXmlStreamWriter * xmlWriter, PipelineItem _pipeline)
{
    xmlWriter->writeStartElement(FBS_IMAGE_PIPELINE_CATEGORY_TAG);
    xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_CATEGORY_NAME_ATT,_pipeline.getCategoryName());
    xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_CATEGORY_ICONPATH_ATT,_pipeline.getIconPath());
    xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_CATEGORY_DESCRIPTION_ATT,_pipeline.getDescription());
    xmlWriter->writeEndElement();
}

void XMLProjectData::writeParametersList(QXmlStreamWriter * xmlWriter, PipelineItem _pipeline)
{
    xmlWriter->writeStartElement(FBS_IMAGE_PIPELINE_PARAMETER_LIST_TAG);

    QList<ConfigurationPipelineItem> _list = _pipeline.getConfigurationItemList();
    for (int i=0; i< _list.size();i++)
    {
        ConfigurationPipelineItem _item = _list.at(i);

        xmlWriter->writeStartElement(FBS_IMAGE_PIPELINE_PARAMETER_TAG);
        xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_PARAMETER_NAME_ATT,_item.getOptionName());
        xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_PARAMETER_TYPE_ATT,_item.getOptionType());
        xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_PARAMETER_DEFAULT_VALUE_ATT,_item.getOptionDefaultValue());
        xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_PARAMETER_METHOD_NAME_ATT,_item.getMethodName());
        xmlWriter->writeAttribute(FBS_IMAGE_PIPELINE_PARAMETER_CURRENT_VALUE_ATT,_item.getCurrentValue());

        xmlWriter->writeEndElement();
    }

    xmlWriter->writeEndElement();
}

void XMLProjectData::writeProjectData(QXmlStreamWriter * xmlWriter, QString _projectPath, QString _projectName)
{
    xmlWriter->writeStartElement(FBS_PROJECT_TAG);
    xmlWriter->writeAttribute(FBS_PROJECT_PATH_ATT,_projectPath);
    xmlWriter->writeAttribute(FBS_PROJECT_NAME_ATT,_projectName);
    xmlWriter->writeEndElement();

}

void XMLProjectData::writeImageData(QXmlStreamWriter *xmlWriter, QString _imgPath, QString _imName, QString _imgPrefix)
{
    xmlWriter->writeStartElement(FBS_IMAGE_TAG);
    xmlWriter->writeAttribute(FBS_IMAGE_PATH_ATT,_imgPath);
    xmlWriter->writeAttribute(FBS_IMAGE_NAME_ATT,_imName);
    xmlWriter->writeAttribute(FBS_IMAGE_PREFIX_NAME_ATT,_imgPrefix);
    xmlWriter->writeEndElement();

}

