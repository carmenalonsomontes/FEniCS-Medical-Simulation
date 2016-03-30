#ifndef USERSESSIONDATA_H
#define USERSESSIONDATA_H

#include "GUI_Module/Data/FBS_Project/fbs_projectdata.h"
class UserSessionData
{
public:
    UserSessionData();



private:

    FBS_ProjectData _currentProjectData;

    void loadCurrentProjectData();

};

#endif // USERSESSIONDATA_H
