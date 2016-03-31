#ifndef MENUBUILDER_H
#define MENUBUILDER_H

#include <QMainWindow>
#include <QObject>
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "GUI_Module/Data/FBS_Project/fbs_projectdata.h"
#include "GUI_Module/Session/usersessiondata.h"


class MenuBuilder : public QObject
{
     Q_OBJECT



public:
    MenuBuilder();
    ~MenuBuilder();

    void registerWindow(QMainWindow *parent);
    void registerProjectData(FBS_ProjectData *projectData);
    void registerUserSessionData(UserSessionData * userSessionData);
    void launchMenuAction(int _action);


signals:
     void updateStatusBarUI(const QString text);
     void enableCloseProjectUI(bool val);
     void enableSaveProjectUI(bool val);
     void updateRecentProjectList(const QString text);


protected:
    QMainWindow * _registered_window_parent;
    FBS_ProjectData * _registered_project_data;
    UserSessionData * _registered_user_session_data;

    void updateUI(int _menu);
    void enableUIElements(int _menu);

};

#endif // MENUBUILDER_H
