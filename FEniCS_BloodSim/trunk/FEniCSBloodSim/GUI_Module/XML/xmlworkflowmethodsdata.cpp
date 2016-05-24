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

            addParameterList(xmlReader,&_functionData);
            _funcList.append(_functionData);

        }

        if ((token == QXmlStreamReader::EndElement) &&
                 (QString::compare(xmlReader->name().toString(), WKF_CATEGORY_TAG) == StrEqual))
            _catEnded = true;
    }

    catData->setListFunctions(_funcList);
}


void XMLWorkflowMethodsData::addParameterList( QXmlStreamReader * xmlReader,ImagingWkfFunctionData * functionData)
{

    bool _parameterEnded = false;
    QStringList _parameterNameList;
    QStringList _parameterTypeList;
    QStringList _parameterClassNameList;
    QStringList _parameterDefaultValueList;

    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!_parameterEnded))
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), WKF_FUNCTION_PARAMETER_ITEM_TAG) == StrEqual) )
        {

            QXmlStreamAttributes _elementAtt = xmlReader->attributes();
            if (_elementAtt.hasAttribute(WKF_FUNCTION_PARAMETER_NAME_ATT))
                _parameterNameList.append(_elementAtt.value(WKF_FUNCTION_PARAMETER_NAME_ATT).toString());

            if (_elementAtt.hasAttribute(WKF_FUNCTION_PARAMETER_TYPE_ATT))
                _parameterTypeList.append(_elementAtt.value(WKF_FUNCTION_PARAMETER_TYPE_ATT).toString());


            if (_elementAtt.hasAttribute(WKF_FUNCTION_PARAMETER_CLASSNAME_ATT))
                _parameterClassNameList.append(_elementAtt.value(WKF_FUNCTION_PARAMETER_CLASSNAME_ATT).toString());

            if (_elementAtt.hasAttribute(WKF_FUNCTION_PARAMETER_DEFAULT_VALUE_ATT))
                _parameterDefaultValueList.append(_elementAtt.value(WKF_FUNCTION_PARAMETER_DEFAULT_VALUE_ATT).toString());
        }

        if ((token == QXmlStreamReader::EndElement) &&
                 (QString::compare(xmlReader->name().toString(), WKF_FUNCTION_PARAMETERS_TAG) == StrEqual))
        {
            _parameterEnded = true;
            functionData->setParametersName(_parameterNameList);
            functionData->setParameterClassName(_parameterClassNameList);
            functionData->setParameterType(_parameterTypeList);
            functionData->setDefaultValue(_parameterDefaultValueList);
        }
    }
}

