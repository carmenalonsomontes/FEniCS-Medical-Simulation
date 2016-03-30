#ifndef FILEMENUBUILDER_H
#define FILEMENUBUILDER_H
#include "menubuilder.h"

class FileMenuBuilder: public MenuBuilder
{
public:
    FileMenuBuilder();
    void launchMenuAction(int _action);

private:
    void createNewProject();
    void openProject();


};

#endif // FILEMENUBUILDER_H
