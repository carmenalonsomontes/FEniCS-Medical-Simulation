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
   // bool saveWorkflowMethodsDataFile(FBS_ProjectData * _projectData);
    void readWorkflowMethodsFile(WorkflowData * _wkfData, typeWorkflow selectedWkf);
private:
    void readImagingWorkflowtData( QXmlStreamReader * xmlReader,WorkflowData * _wkfData);
    void addFunctionList( QXmlStreamReader * xmlReader,CategoryWkfData * catData);


 /*   bool createNewFile(QFile * _file,FBS_ProjectData * _projectData);
    void writeProjectData(QXmlStreamWriter * xmlWriter, QString _projectPath, QString _projectName);
    void writeImageData(QXmlStreamWriter *xmlWriter, QString _imgPath, QString _imName, QString _imgPrefix);

    void readProjectData( QXmlStreamReader * xmlReader,FBS_ProjectData * _projectData);*/

};

#endif // XMLWORKFLOWMETHODSDATA_H
