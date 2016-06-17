#ifndef WORKFLOWDEFINES_H
#define WORKFLOWDEFINES_H

#include <QString>

const QString TEMP_FOLDER_WKF_IMAGES="images";
const QString TEMP_WKF_PIPELINE_IMAGE_PREFIX = "tmp_wkf_";



enum LoadUserImage{
    LOAD_USER_IMAGE = 1,
    NOT_LOAD_USER_IMAGE = 0
};


// --------------------------------------------
// Values for position of parameters in the PIPELINE
enum PipelinePosition{
    PARAMETER_NAME = 0,
    PARAMETER_VALUE = 1
};

const QString PARAMETER_SEPARATOR = ";";



const QString WFK_IMAGE_FILE_PATH =":/files/GUI_Module/Forms/Workflow/XML/ImageXmlMethods.xml";

const QString ADD_BUTTON_ICON_FILE_PATH = ":/menu/GUI_Module/images/menu/add.png";
const QString UP_ICON_PATH=":/wkf/images/GUI_Module/images/images/bullet_arrow_up.png";
const QString DOWN_ICON_PATH=":/wkf/images/GUI_Module/images/images/bullet_arrow_down.png";
const QString DELETE_ICON_PATH=":/menu/GUI_Module/images/menu/cross.png";


enum IconType{
    HELP_ICON = 0,
    ADD_ICON = 1,
    UP_ICON = 2,
    DOWN_ICON = 3,
    DELETE_ICON = 4
};

enum typeWorkflow{
    IMAGING_WKF = 0,
    FENICS_WKF = 1
};

enum ParamConfigurationTableCols{
    PARAM_TYPE_COLUMN = 0,
    PARAM_NAME_COLUMN = 1,
    PARAM_VALUE_COLUMN = 2,
    PARAM_METHOD_NAME_COLUMN = 3
};



enum genericWkfDialog{
    CHECKABLE_COLUMN = 0,
    ONLY_DESC_COLUMN = 1,
    UP_ICON_COLUMN = 2,
    DOWN_ICON_COLUMN = 3,
    DELETE_ICON_COLUMN = 4// In the case we only want a column
};


enum ImagingWkfDialog{
    METHOD_NAME_COLUMN = 0,
    ACTION_COLUMN = 1
};


// XML DEFINEs
const QString WKF_IMAGING_TAG = "imaging-functions";
const QString WKF_CATEGORY_TAG = "category";
const QString WKF_CATEGORY_NAME_ATT = "name";
const QString WKF_CATEGORY_ICON_ATT = "icon";
const QString WKF_FUNCTION_TAG = "function";

const QString WKF_FUNCTION_NAME_ATT = "name";
const QString WKF_FUNCTION_CLASSNAME_ATT = "classname";
const QString WKF_FUNCTION_DESCRIPTION_ATT = "description";
const QString WKF_FUNCTION_PARAMETERS_TAG = "parameters";
const QString WKF_FUNCTION_PARAMETER_ITEM_TAG = "parameter";
const QString WKF_FUNCTION_PARAMETER_NAME_ATT = "name";
const QString WKF_FUNCTION_PARAMETER_METHOD_NAME_ATT = "methodname";
const QString WKF_FUNCTION_PARAMETER_TYPE_ATT = "type";
const QString WKF_FUNCTION_PARAMETER_DEFAULT_VALUE_ATT = "default_value";



#endif // WORKFLOWDEFINES_H
