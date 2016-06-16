#ifndef XMLFENICSINFORMATIONDATA_H
#define XMLFENICSINFORMATIONDATA_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "GUI_Module/Data/FEniCSData/fenicsxmldata.h"
#include "GUI_Module/Data/FEniCSData/categoryfenicsdata.h"
#include "GUI_Module/Data/FEniCSData/combovaluesfenicsdata.h"
#include "GUI_Module/Data/FEniCSData/functionfenicsdata.h"
#include "GUI_Module/Data/FEniCSData/parameterfenicsdata.h"

class XmlFEniCSInformationData
{
public:
    XmlFEniCSInformationData();

    void readFEniCSFile(FEniCSXMLData * _fenicsData);
private:
    void readFEniCSData( QXmlStreamReader * xmlReader,FEniCSXMLData * _fenicsData);
    void addFunctionList( QXmlStreamReader * xmlReader,CategoryFEniCSData * catData);
    void addParameterList( QXmlStreamReader * xmlReader,FunctionFEniCSData * functionData);
    void addValuesList( QXmlStreamReader * xmlReader,ParameterFEniCSData * parameterData);


};

#endif // XMLFENICSINFORMATIONDATA_H
