#ifndef USERSESSIONDATA_H
#define USERSESSIONDATA_H

#include <QStringList>
class UserSessionData
{
public:
    UserSessionData();

    void loadRecentProjectList();
    void saveRecentProjectList();
    bool updateRecentProjectList(QString _projectPath);


    QStringList getRecentProjectList();
    QStringList getRecentProjectNameList();

    bool isEmpty();
    void setFileNamePath(QString _path);
    void resetProjects();


private:
    QStringList _recentProjectList;
    QStringList _recentProjectNameList;
    QString _filePath;


};

#endif // USERSESSIONDATA_H
