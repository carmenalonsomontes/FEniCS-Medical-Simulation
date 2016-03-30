#ifndef MENUBUILDER_H
#define MENUBUILDER_H

#include <QMainWindow>
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "GUI_Module/Data/FBS_Project/fbs_projectdata.h"


class MenuBuilder
{
public:
    MenuBuilder();
    ~MenuBuilder();

    void registerWindow(QMainWindow *parent);
    void registerProjectData(FBS_ProjectData *projectData);

    void launchMenuAction(int _action);

protected:
    QMainWindow * _registered_window_parent;
    FBS_ProjectData * _registered_project_data;


};

#endif // MENUBUILDER_H
