#include "xmlworkflowmethodsdata.h"

#include "GUI_Module/Data/WorkflowData/imagingwkffunctiondata.h"
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include <QFile>

XMLWorkflowMethodsData::XMLWorkflowMethodsData()
{
}
void XMLWorkflowMethodsData::readWorkflowMethodsFile(WorkflowData * _wkfData, typeWorkflow selectedWkf)
{
    bool _fileRead = true;
    QString path;
    if (selectedWkf == IMAGING_WKF)
        path = WFK_IMAGE_FILE_PATH;
    if (path.isEmpty())
        return;

    QFile _file(path);
    _fileRead = _file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (_fileRead)
    {
        QXmlStreamReader xmlReader(&_file);
        if (selectedWkf == IMAGING_WKF)
            readImagingWorkflowtData(&xmlReader,_wkfData);
    }
    _file.close();
}

void XMLWorkflowMethodsData::readImagingWorkflowtData( QXmlStreamReader * xmlReader,WorkflowData * _wkfData)
{

    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()))
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if (token == QXmlStreamReader::StartDocument) continue;
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), WKF_CATEGORY_TAG) == StrEqual) )
        {
                QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                 CategoryWkfData _newCategoryObj;
                if (_elementAtt.hasAttribute(WKF_CATEGORY_NAME_ATT))
                     _newCategoryObj.setCategoryName(_elementAtt.value(WKF_CATEGORY_NAME_ATT).toString());
                if (_elementAtt.hasAttribute(WKF_CATEGORY_ICON_ATT))
                     _newCategoryObj.setIconPath(_elementAtt.value(WKF_CATEGORY_ICON_ATT).toString());
                //Note:  We don't suppose to have empty categories
                addFunctionList(xmlReader,&_newCategoryObj);
                _wkfData->addCategory(_newCategoryObj);
        }

    }

}


//Note:  We don't allow to have empty categories
void XMLWorkflowMethodsData::addFunctionList( QXmlStreamReader * xmlReader,CategoryWkfData * catData)
{
    bool _catEnded = false;
    QList<ImagingWkfFunctionData> _funcList;
    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!_catEnded))
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), WKF_FUNCTION_TAG) == StrEqual) )
        {

            ImagingWkfFunctionData  _functionData;// = new ImagingWkfFunctionData();
            QXmlStreamAttributes _elementAtt = xmlReader->attributes();
            if (_elementAtt.hasAttribute(WKF_FUNCTION_NAME_ATT))
                _functionData.setName(_elementAtt.value(WKF_FUNCTION_NAME_ATT).toString());

            if (_elementAtt.hasAttribute(WKF_FUNCTION_CLASSNAME_ATT))
                _functionData.setClassName(_elementAtt.value(WKF_FUNCTION_CLASSNAME_ATT).toString());

            if (_elementAtt.hasAttribute(WKF_FUNCTION_DESCRIPTION_ATT))
                _functionData.setDescription(_elementAtt.value(WKF_FUNCTION_DESCRIPTION_ATT).toString());

            if (_elementAtt.hasAttribute(WKF_FUNCTION_PARAMETERS_ATT))
                _functionData.setParameters(_elementAtt.value(WKF_FUNCTION_PARAMETERS_ATT).toString());

            _funcList.append(_functionData);

        }

        if ((token == QXmlStreamReader::EndElement) &&
                 (QString::compare(xmlReader->name().toString(), WKF_CATEGORY_TAG) == StrEqual))
            _catEnded = true;
    }

    catData->setListFunctions(_funcList);
}

