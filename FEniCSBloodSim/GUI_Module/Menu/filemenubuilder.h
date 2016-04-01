#ifndef FILEMENUBUILDER_H
#define FILEMENUBUILDER_H
#include "menubuilder.h"

class FileMenuBuilder: public MenuBuilder
{
public:
    FileMenuBuilder();
    void launchMenuAction(int _action);
    void launchMenuAction(int _action,QString _pathfile);


private:
    int createNewProject();
    int openProject();
    bool saveas_project();
    int openProject(QString projectPath);

    // UI connection functions
    void updateUI(int _menu);
    void enableUIElements(int _menu);
    //void restoreUI();
};

#endif // FILEMENUBUILDER_H
