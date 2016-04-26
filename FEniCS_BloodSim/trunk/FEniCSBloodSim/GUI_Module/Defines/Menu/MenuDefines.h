#ifndef MENUDEFINES_H
#define MENUDEFINES_H
#include <QString>

// VIEW ORIENTATION
enum Orientation_Im{
    SAGITTAL_YZ = 0,
    CORONAL_XZ = 1,
    AXIAL_XY = 2
};

enum Slicer_no{
    AXIAL_SLICER = 0,
    SAGITTAL_SLICER = 1,
    CORONAL_SLICER =2
};


// Image Processing MENU UI
enum ImProcMenuUI{
    EYE_OPEN = 0,
    EYE_CLOSED = 1
};

enum ImProcTableCols{
    EYE_COLUMN = 0,
    DESC_COLUMN = 1
};


// FILES NAMEs

const QString PREFERENCES_FILE_NAME="fbs_preferences.fbs";


enum TabIndex{
    IMAGE_TAB_INDEX = 0,
    MESH_TAB_INDEX = 1,
    FENICS_TAB_INDEX = 2,
    SIMULATION_TAB_INDEX = 3,
    VISUALIZATION_TAB_INDEX = 4
};


enum Success_Failure_func{
    SUCCESS = 0,
    FAILURE = 1
};

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

enum SimulationMenuVar{
    IMAGE_PROCESSING = 0,
    MESH_TOOL = 1,
    FENICS_TOOL = 2,
    VISUALIZATION_TOOL = 3

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

// PReferences XML Tags
const QString FBS_PREFERENCES_TAG = "fbs_preferences";
const QString FBS_RECENT_PROJECT_LIST_TAG = "fbs_recent_project_list";
const QString FBS_ITEM_PROJECT_TAG = "fbs_item_project";
const QString FBS_ITEM_PROJECT_PATH_ATT = "fbs_item_project_path";
const QString FBS_TOOL_PATH_TAG="tool_path";
const QString FBS_MESH_TOOL_PATH_TAG="mesh_tool";
const QString FBS_MESH_TOOL_PATH_ATT="mesh-path";
const QString FBS_FENICS_TOOL_PATH_TAG="fenics-tool";
const QString FBS_FENICS_TOOL_PATH_ATT="fenics-path";



#endif // MENUDEFINES_H
