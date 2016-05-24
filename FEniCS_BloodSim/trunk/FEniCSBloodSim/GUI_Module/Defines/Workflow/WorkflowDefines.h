#ifndef WORKFLOWDEFINES_H
#define WORKFLOWDEFINES_H

#include <QString>

const QString WFK_IMAGE_FILE_PATH =":/files/GUI_Module/Forms/Workflow/XML/ImageXmlMethods.xml";

enum typeWorkflow{
    IMAGING_WKF = 0,
    FENICS_WKF = 1
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
const QString WKF_FUNCTION_PARAMETER_CLASSNAME_ATT = "classname";
const QString WKF_FUNCTION_PARAMETER_TYPE_ATT = "type";
const QString WKF_FUNCTION_PARAMETER_DEFAULT_VALUE_ATT = "default_value";



#endif // WORKFLOWDEFINES_H
