#ifndef IMAGEPROCESSINGSIMMENU_H
#define IMAGEPROCESSINGSIMMENU_H
#include "GUI_Module/Menu/menubuilder.h"

class ImageProcessingSimMenu: public MenuBuilder
{
public:
    ImageProcessingSimMenu();
    void launchMenuAction(int _action);

protected:
  /*  void updateImagingUI(int _menu);
    void enableImagingUIElements(int _menu);*/

private:
    void openImagingDialog();

};

#endif // IMAGEPROCESSINGSIMMENU_H
