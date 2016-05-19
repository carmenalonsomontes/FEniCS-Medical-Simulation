#ifndef XMLWORKFLOWMETHODSDATA_H
#define XMLWORKFLOWMETHODSDATA_H
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "GUI_Module/Data/WorkflowData/workflowdata.h"
#include "GUI_Module/Defines/Workflow/WorkflowDefines.h"
#include "GUI_Module/Data/WorkflowData/categorywkfdata.h"

class XMLWorkflowMethodsData
{
public:
    XMLWorkflowMethodsData();
    void readWorkflowMethodsFile(WorkflowData * _wkfData, typeWorkflow selectedWkf);
private:
    void readImagingWorkflowtData( QXmlStreamReader * xmlReader,WorkflowData * _wkfData);
    void addFunctionList( QXmlStreamReader * xmlReader,CategoryWkfData * catData);



};

#endif // XMLWORKFLOWMETHODSDATA_H
