#ifndef ABOUTMENUBUILDER_H
#define ABOUTMENUBUILDER_H
#include "menubuilder.h"

class AboutMenuBuilder: public MenuBuilder
{
public:
    AboutMenuBuilder();
    void launchMenuAction(int _action);
};

#endif // ABOUTMENUBUILDER_H
