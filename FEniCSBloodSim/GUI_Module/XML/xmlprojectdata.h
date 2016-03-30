#ifndef XMLPROJECTDATA_H
#define XMLPROJECTDATA_H
#include <QString>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "../Data/FBS_Project/fbs_projectdata.h"





class XMLProjectData
{
public:
    XMLProjectData();
    bool saveProjectDataFile(FBS_ProjectData * _projectData);
    void readProjectDataFile(FBS_ProjectData * _projectData, QString path);

private:

    bool createNewFile(QFile * _file,FBS_ProjectData * _projectData);
    bool updateFile(QFile * _file, FBS_ProjectData * _projectData);
    void writeProjectData(QXmlStreamWriter * xmlWriter, QString _projectPath, QString _projectName);
    void writeImageData(QXmlStreamWriter *xmlWriter, QString _imgPath, QString _imName, QString _imgPrefix);

    void readProjectData( QXmlStreamReader * xmlReader,FBS_ProjectData * _projectData);


};

#endif // XMLPROJECTDATA_H
