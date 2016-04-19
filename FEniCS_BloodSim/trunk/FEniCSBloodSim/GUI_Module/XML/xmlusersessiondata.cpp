#include "xmlusersessiondata.h"

#include "../Defines/Menu/MenuDefines.h"

XMLUserSessionData::XMLUserSessionData()
{
}


bool XMLUserSessionData::saveUserSessionData(UserSessionData * _userSessionData, QString path)
{

    QFile _file(path);
    bool _success = false;

   if (_file.exists())
       _file.remove();
   _success= createNewFile(&_file,_userSessionData);

   return _success;
}

bool XMLUserSessionData::createNewFile(QFile * _file,UserSessionData * _userSessionData)
{
    bool _success = false;
    _success = _file->open(QIODevice::ReadWrite);
    if (_success)
    {

          QXmlStreamWriter xmlWriter(_file);
          xmlWriter.setAutoFormatting(true);
          xmlWriter.writeStartDocument();

          xmlWriter.writeStartElement(FBS_PREFERENCES_TAG);

          writeRecentProjectList(&xmlWriter,_userSessionData);

          writeToolPaths(&xmlWriter,_userSessionData);

          // Closing the root and the document
          xmlWriter.writeEndElement();
          xmlWriter.writeEndDocument();
          _file->close();
    }

   return _success;

}

void XMLUserSessionData::writeToolPaths(QXmlStreamWriter * xmlWriter, UserSessionData * _userSessionData)
{
    xmlWriter->writeStartElement(FBS_TOOL_PATH_TAG);

    // MESH TOOL PATH
    xmlWriter->writeStartElement(FBS_MESH_TOOL_PATH_TAG);
    xmlWriter->writeAttribute(FBS_MESH_TOOL_PATH_ATT,_userSessionData->getMeshToolPath());
    xmlWriter->writeEndElement();


    // FENICS TOOL PATH
    xmlWriter->writeStartElement(FBS_FENICS_TOOL_PATH_TAG);
    xmlWriter->writeAttribute(FBS_FENICS_TOOL_PATH_ATT,_userSessionData->getFenicsToolPath());
    xmlWriter->writeEndElement();

    xmlWriter->writeEndElement();
}




void XMLUserSessionData::writeRecentProjectList(QXmlStreamWriter * xmlWriter, UserSessionData * _userSessionData)
{
    QStringList _projectList = _userSessionData->getRecentProjectList();
    _projectList.removeDuplicates();
    xmlWriter->writeStartElement(FBS_RECENT_PROJECT_LIST_TAG);
    if (!_projectList.isEmpty())
        for (int i=0; i <_projectList.size();i++)
        {
            xmlWriter->writeStartElement(FBS_ITEM_PROJECT_TAG);
            xmlWriter->writeAttribute(FBS_ITEM_PROJECT_PATH_ATT,_projectList.at(i));
            xmlWriter->writeEndElement();
        }
    xmlWriter->writeEndElement();

}

void XMLUserSessionData::readUserSessionData(UserSessionData * _userSessionData, QString path)
{
    bool _fileRead = true;
    QFile _file(path);
    _fileRead = _file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (_fileRead)
    {
        QXmlStreamReader xmlReader(&_file);
        readRecentProjectList(&xmlReader,_userSessionData);
        readToolPaths(&xmlReader,_userSessionData);
    }
    _file.close();
}

void XMLUserSessionData::readToolPaths( QXmlStreamReader * xmlReader,UserSessionData * _userSessionData)
{
    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()))
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if (token == QXmlStreamReader::StartDocument) continue;

        // Mesh tool
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), FBS_MESH_TOOL_PATH_TAG) == StrEqual) )
        {
                QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                if (_elementAtt.hasAttribute(FBS_MESH_TOOL_PATH_ATT))
                    _userSessionData->setMeshToolPath(_elementAtt.value(FBS_MESH_TOOL_PATH_ATT).toString());
        }
        // Fenics tool
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), FBS_FENICS_TOOL_PATH_TAG) == StrEqual) )
        {
                QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                if (_elementAtt.hasAttribute(FBS_FENICS_TOOL_PATH_ATT))
                   _userSessionData->setFenicsToolPath(_elementAtt.value(FBS_FENICS_TOOL_PATH_ATT).toString());
        }
    }

}

void XMLUserSessionData::readRecentProjectList( QXmlStreamReader * xmlReader,UserSessionData * _userSessionData)
{
    bool _endProjects = false;
    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!_endProjects))
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if (token == QXmlStreamReader::StartDocument) continue;
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), FBS_ITEM_PROJECT_TAG) == StrEqual) )
        {
                QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                if (_elementAtt.hasAttribute(FBS_ITEM_PROJECT_PATH_ATT))
                    _userSessionData->updateRecentProjectList(_elementAtt.value(FBS_ITEM_PROJECT_PATH_ATT).toString());
        }
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), FBS_TOOL_PATH_TAG) == StrEqual) )
            _endProjects = true;
    }
}






