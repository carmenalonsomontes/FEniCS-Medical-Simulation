#include "menubuilder.h"



MenuBuilder::MenuBuilder()
{

}
MenuBuilder::~MenuBuilder()
{

}

void MenuBuilder::registerWindow(QMainWindow *parent)
{
    // Registering the parent window
    _registered_window_parent = parent;
}

void MenuBuilder::registerProjectData(FBS_ProjectData *projectData)
{
    _registered_project_data = projectData;
}
void MenuBuilder::registerUserSessionData(UserSessionData * userSessionData)
{
    _registered_user_session_data = userSessionData;
}

