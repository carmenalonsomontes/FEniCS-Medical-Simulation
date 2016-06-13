#ifndef USERSESSIONDATA_H
#define USERSESSIONDATA_H

#include <QStringList>
#include <QList>

#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "userimageinfosession.h"
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


    void setImageTabsInfo(int noImageRow, ImageTabs sliceType, int noSlice, int maxSlice, int minSlice);
    UserImageInfoSession getImageTabsInfo(int noImageRow, ImageTabs sliceType );
    void clearImageTabsInfo();
    bool isEmptyImageTabInfo();
    bool existImageInfo(int noImageRow, ImageTabs sliceType );

private:
    QStringList _recentProjectList;
    QStringList _recentProjectNameList;
    QString _filePath;
    QString _meshToolPath;
    QString _fenicsToolPath;

    QList<UserImageInfoSession> _imagesTabInfo; // De momento no se guarda en fichero ...


};

#endif // USERSESSIONDATA_H
