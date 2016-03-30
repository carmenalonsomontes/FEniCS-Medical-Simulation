#ifndef MENUDEFINES_H
#define MENUDEFINES_H
#include <QString>
// File Extensions

const QString FBS_FILE_PROJECT_EXT  = "fbs";

const int StrEqual = 0;

// MENUS
enum MenusVar{
     FILE_MENU = 0
};


// FILE MENU

enum FileMenuVar{
      NEW_PROJECT      = 0,
      OPEN_PROJECT     = 1,
      RECENT_PROJECTS  = 2,
      SAVE_PROJECT     = 3,
      SAVEAS_PROJECT   = 4,
      CLOSE_PROJECT    = 5,
      EXIT_APP         = 6,
      EXIT_MAINWINDOW  = 7
};


// XML Defines
const QString FBS_GUI_TAG = "fbs_gui";
const QString FBS_PROJECT_TAG = "fbs_project";
const QString FBS_PROJECT_PATH_ATT = "fbs_project_path";
const QString FBS_PROJECT_NAME_ATT = "fbs_project_name";
const QString FBS_IMAGE_TAG = "fbs_image";
const QString FBS_IMAGE_PATH_ATT = "fbs_image_path";
const QString FBS_IMAGE_NAME_ATT = "fbs_image_name";
const QString FBS_IMAGE_PREFIX_NAME_ATT = "fbs_image_prefix_name";


#endif // MENUDEFINES_H
