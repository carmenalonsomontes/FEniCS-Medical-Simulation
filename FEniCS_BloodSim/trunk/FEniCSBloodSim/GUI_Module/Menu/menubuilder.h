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
    // File Menu Builder
     void updateStatusBarUI(const QString text);
     void updateStatusBarUI2(const QString text);
     void enableCloseProjectUI(bool val);
     void enableSaveProjectUI(bool val);
     void updateRecentProjectList(const QString text);
     void restoreUI();
     void enableMedicalImagingFrameUI(bool val);

     // Image Processing Builder
     void updateImagingDialogUI(const QString text);


protected:
    QMainWindow * _registered_window_parent;
    FBS_ProjectData * _registered_project_data;
    UserSessionData * _registered_user_session_data;


};

#endif // MENUBUILDER_H
