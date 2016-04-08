#ifndef EDITMENUBUILDER_H
#define EDITMENUBUILDER_H
#include "menubuilder.h"

class EditMenuBuilder: public MenuBuilder
{
public:
    EditMenuBuilder();
    void launchMenuAction(int _action);
};

#endif // EDITMENUBUILDER_H
