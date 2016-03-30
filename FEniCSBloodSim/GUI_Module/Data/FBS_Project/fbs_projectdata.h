#ifndef FBS_PROJECTDATA_H
#define FBS_PROJECTDATA_H

#include <QString>



class FBS_ProjectData
{
public:
    FBS_ProjectData();

    QString getProjectPath();
    QString getProjectName();

    QString getImPath();
    QString getImPrefixSeries();
    QString getImName();


    void setProjectPath(QString _path);
    void setProjectName(QString _name);
    void setImPath(QString _path);
    void setImPrefixSeries(QString _prefixSeries);
    void setImName(QString _name);

    void saveProjectPathName(QString _projectPathName);
    void saveProjectInfoToFile();
    void readProjectInfoFromFile(QString _projectFilePath);

    void clearData();

    bool isEmpty();

private:
    QString _projectPath;
    QString _projectName;

    QString _imPath;
    QString _imPrefixSeries;
    QString _imName;


    void initializeVariables();

};

#endif // FBS_PROJECTDATA_H
