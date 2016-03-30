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
    if (!_file.exists())
        _success= createNewFile(&_file,_projectData);
    else
        _success = updateFile(&_file,_projectData);

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
    }

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

       // Closing the root and the document
       xmlWriter.writeEndElement();
       xmlWriter.writeEndDocument();
       _file->close();
   }

   return _success;
}





bool XMLProjectData::updateFile(QFile * _file,FBS_ProjectData * _projectData)
{
    bool _success = false;
    return _success;
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

