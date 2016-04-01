#ifndef IMAGEPROCESSINGSIMMENU_H
#define IMAGEPROCESSINGSIMMENU_H
#include "GUI_Module/Menu/menubuilder.h"

class ImageProcessingSimMenu: public MenuBuilder
{
public:
    ImageProcessingSimMenu();
    void launchMenuAction(int _action);
    void launchMenuAction(int _action,QString _pathfile);


private:
    QString _imgPath;

    void updateImagingDialog();
    void openImagingDialog();
};

#endif // IMAGEPROCESSINGSIMMENU_H
