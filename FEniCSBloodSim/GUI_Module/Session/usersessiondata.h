#ifndef USERSESSIONDATA_H
#define USERSESSIONDATA_H

#include <QStringList>
class UserSessionData
{
public:
    UserSessionData();

    void loadAllData();
    void saveAllData();

    //void loadRecentProjectList();
    //void saveRecentProjectList();
    bool updateRecentProjectList(QString _projectPath);


    QStringList getRecentProjectList();
    QStringList getRecentProjectNameList();

    bool isEmptyRecentProjects();
    void setFileNamePath(QString _path);
    void resetProjects();


    QString getMeshToolPath();
    void setMeshToolPath(QString toolpath);
    QString getFenicsToolPath();
    void setFenicsToolPath(QString toolpath);



private:
    QStringList _recentProjectList;
    QStringList _recentProjectNameList;
    QString _filePath;
    QString _meshToolPath;
    QString _fenicsToolPath;


};

#endif // USERSESSIONDATA_H
