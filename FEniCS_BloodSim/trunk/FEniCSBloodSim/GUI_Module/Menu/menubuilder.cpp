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


/**
 * @brief Establish the main contents of the status bar (logo, labels, etc)
 *
 */
/*void MainWindow::setUpStatusBar()
{
    QPixmap *pixmap = new QPixmap (":/logo/images/icon_8x16.png");
    QLabel *iconLbl = new QLabel;
    iconLbl->setPixmap(*pixmap);
    statusBar()->addPermanentWidget(iconLbl);
    delete pixmap;
}
*/
