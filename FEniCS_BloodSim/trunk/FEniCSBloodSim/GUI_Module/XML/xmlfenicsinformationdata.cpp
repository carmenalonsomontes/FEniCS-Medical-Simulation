#include "xmlfenicsinformationdata.h"

#include "GUI_Module/Defines/FEniCS/FEniCSDefines.h"
#include "GUI_Module/Defines/Menu/MenuDefines.h"

#include <QFile>

XmlFEniCSInformationData::XmlFEniCSInformationData()
{
}
void XmlFEniCSInformationData::readFEniCSFile(FEniCSXMLData * _fenicsData)
{
    bool _fileRead = true;
    QString path;
    path = FENICS_FILE_PATH;
    if (path.isEmpty())
        return;

    QFile _file(path);
    _fileRead = _file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (_fileRead)
    {
        QXmlStreamReader xmlReader(&_file);
        readFEniCSData(&xmlReader,_fenicsData);
    }
    _file.close();

}

void XmlFEniCSInformationData::readFEniCSData( QXmlStreamReader * xmlReader,FEniCSXMLData * _fenicsData)
{
    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()))
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if (token == QXmlStreamReader::StartDocument) continue;
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), FENICS_CATEGORY_TAG) == StrEqual) )
        {
                QXmlStreamAttributes _elementAtt = xmlReader->attributes();
                 CategoryFEniCSData _newCategoryObj;
                if (_elementAtt.hasAttribute(FENICS_CATEGORY_NAME_ATT))
                     _newCategoryObj.setCategoryName(_elementAtt.value(FENICS_CATEGORY_NAME_ATT).toString());
                if (_elementAtt.hasAttribute(FENICS_CATEGORY_KEY_INPUT_ATT))
                     _newCategoryObj.setKeyInput(_elementAtt.value(FENICS_CATEGORY_KEY_INPUT_ATT).toString());
                //Note:  We don't suppose to have empty categories
                addFunctionList(xmlReader,&_newCategoryObj);
                _fenicsData->addCategory(_newCategoryObj);
        }

    }

}

void XmlFEniCSInformationData::addFunctionList( QXmlStreamReader * xmlReader,CategoryFEniCSData * catData)
{

    bool _catEnded = false;
    QList<FunctionFEniCSData> _funcList;
    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!_catEnded))
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), FENICS_FUNCTION_TAG) == StrEqual) )
        {

            FunctionFEniCSData  _functionData;
            QXmlStreamAttributes _elementAtt = xmlReader->attributes();
            if (_elementAtt.hasAttribute(FENICS_FUNCTION_NAME_ATT))
                _functionData.setFunctionName(_elementAtt.value(FENICS_FUNCTION_NAME_ATT).toString());

            if (_elementAtt.hasAttribute(FENICS_FUNCTION_CLASSNAME_ATT))
                _functionData.setFenicsFunctionName(_elementAtt.value(FENICS_FUNCTION_CLASSNAME_ATT).toString());

            if (_elementAtt.hasAttribute(FENICS_FUNCTION_DESCRIPTION_ATT))
                _functionData.setDescription(_elementAtt.value(FENICS_FUNCTION_DESCRIPTION_ATT).toString());

            addParameterList(xmlReader,&_functionData);
            _funcList.append(_functionData);

        }

        if ((token == QXmlStreamReader::EndElement) &&
                 (QString::compare(xmlReader->name().toString(), FENICS_CATEGORY_TAG) == StrEqual))
            _catEnded = true;
    }

    catData->setListFunctions(_funcList);

}

void XmlFEniCSInformationData::addParameterList( QXmlStreamReader * xmlReader,FunctionFEniCSData * functionData)
{
    bool _parameterEnded = false;

    QList<ParameterFEniCSData> _parameterList;

    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!_parameterEnded))
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), FENICS_FUNCTION_PARAMETER_ITEM_TAG) == StrEqual) )
        {

            ParameterFEniCSData _parameterData;
            QXmlStreamAttributes _elementAtt = xmlReader->attributes();
            if (_elementAtt.hasAttribute(FENICS_FUNCTION_PARAMETER_NAME_ATT))
                _parameterData.setName(_elementAtt.value(FENICS_FUNCTION_PARAMETER_NAME_ATT).toString());

            if (_elementAtt.hasAttribute(FENICS_FUNCTION_PARAMETER_METHOD_NAME_ATT))
                _parameterData.setMethodName(_elementAtt.value(FENICS_FUNCTION_PARAMETER_METHOD_NAME_ATT).toString());


            if (_elementAtt.hasAttribute(FENICS_FUNCTION_PARAMETER_DESCRIPTION_ATT))
                _parameterData.setDescription(_elementAtt.value(FENICS_FUNCTION_PARAMETER_DESCRIPTION_ATT).toString());

            if (_elementAtt.hasAttribute(FENICS_FUNCTION_PARAMETER_DEFAULT_VALUE_ATT))
                _parameterData.setDefaultValue(_elementAtt.value(FENICS_FUNCTION_PARAMETER_DEFAULT_VALUE_ATT).toString());

            if (_elementAtt.hasAttribute(FENICS_FUNCTION_PARAMETER_INPUT_ATT))
               _parameterData.setInput(_elementAtt.value(FENICS_FUNCTION_PARAMETER_INPUT_ATT).toString());

            if (_elementAtt.hasAttribute(FENICS_FUNCTION_PARAMETER_IS_OPTIONAL_ATT))
                if (QString::compare(_elementAtt.value(FENICS_FUNCTION_PARAMETER_IS_OPTIONAL_ATT).toString(),"YES") == 0)
                    _parameterData.setOptional(true);            

            addValuesList(xmlReader,&_parameterData);
            _parameterList.append(_parameterData);
        }



        if ((token == QXmlStreamReader::EndElement) &&
                 (QString::compare(xmlReader->name().toString(), FENICS_FUNCTION_PARAMETERS_TAG) == StrEqual))
            _parameterEnded = true;

    }

    functionData->setParameterList(_parameterList);
}


void XmlFEniCSInformationData::addValuesList( QXmlStreamReader * xmlReader,ParameterFEniCSData * parameterData)
{
    bool _valuesEnded = false;

    QList<ComboValuesFEniCSData> _comboList;

    while ( (!xmlReader->atEnd()) && (!xmlReader->hasError()) && (!_valuesEnded))
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();

        if ((token == QXmlStreamReader::EndElement) &&
                 (QString::compare(xmlReader->name().toString(), FENICS_FUNCTION_PARAMETER_ITEM_TAG) == StrEqual))
            _valuesEnded = true;
        if ((token == QXmlStreamReader::EndElement) &&
                 (QString::compare(xmlReader->name().toString(), FENICS_FUNCTION_PARAMETERS_TAG) == StrEqual))
            _valuesEnded = true;

        if ((token == QXmlStreamReader::StartElement) &&
                (QString::compare(xmlReader->name().toString(), FENICS_FUNCTION_COMBO_VALUE_TAG) == StrEqual) )
        {
            ComboValuesFEniCSData _comboValue;
            QXmlStreamAttributes _elementAtt = xmlReader->attributes();
            if (_elementAtt.hasAttribute(FENICS_FUNCTION_COMBO_VALUE_NAME_ATT))
                _comboValue.setName(_elementAtt.value(FENICS_FUNCTION_COMBO_VALUE_NAME_ATT).toString());

            if (_elementAtt.hasAttribute(FENICS_FUNCTION_COMBO_VALUE_FENICS_VALUE_ATT))
                _comboValue.setFenicsValue(_elementAtt.value(FENICS_FUNCTION_COMBO_VALUE_FENICS_VALUE_ATT).toString());

            _comboList.append(_comboValue);
        }

        if ((token == QXmlStreamReader::EndElement) &&
                 (QString::compare(xmlReader->name().toString(), FENICS_FUNCTION_COMBO_VALUES_TAG) == StrEqual))
            _valuesEnded = true;



    }

    parameterData->setComboValuesList(_comboList);
}
