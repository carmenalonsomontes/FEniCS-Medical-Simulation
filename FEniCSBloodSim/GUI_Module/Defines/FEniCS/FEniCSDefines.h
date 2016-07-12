#ifndef FENICSDEFINES_H
#define FENICSDEFINES_H

#include <QString>


enum FENICSDialog{
    FENICS_METHOD_NAME_COLUMN = 0,
    FENICS_ACTION_COLUMN = 1
};

enum FEnicsConfigurationTableCols{
    FENICS_PARAM_NAME = 0,
    FENICS_PARAM_VALUE = 1
};

enum FEniCSPipelineDialog{
    CATEGORY_COLUMN = 0,
    OPERATION_COLUMN = 1,
    FENICS_UP_ICON_COLUMN = 2,
    FENICS_DOWN_ICON_COLUMN = 3,
    FENICS_DELETE_ICON_COLUMN = 4// In the case we only want a column
};

const QString FENICS_FILE_PATH=":/files/GUI_Module/Forms/FEniCS/XML/FEniCSXmlCodeSnippsets.xml";
const QString FENICS_SIMULATION_FILE_NAME_DEFAULT="fenics_simulation.py";


// XML DEFINEs
const QString FENICS_TAG = "fenics-functions";
const QString FENICS_CATEGORY_TAG = "category";
const QString FENICS_CATEGORY_NAME_ATT = "name";
const QString FENICS_CATEGORY_KEY_INPUT_ATT = "keyInput";

const QString FENICS_FUNCTION_TAG = "function";

const QString FENICS_FUNCTION_NAME_ATT = "name";
const QString FENICS_FUNCTION_CLASSNAME_ATT = "fenicsName";
const QString FENICS_FUNCTION_DESCRIPTION_ATT = "description";


const QString FENICS_FUNCTION_PARAMETERS_TAG = "parameters";
const QString FENICS_FUNCTION_PARAMETER_ITEM_TAG = "parameter";

const QString FENICS_FUNCTION_PARAMETER_NAME_ATT = "name";
const QString FENICS_FUNCTION_PARAMETER_METHOD_NAME_ATT = "methodname";
const QString FENICS_FUNCTION_PARAMETER_DESCRIPTION_ATT = "description";
const QString FENICS_FUNCTION_PARAMETER_INPUT_ATT = "input";
const QString FENICS_FUNCTION_PARAMETER_DEFAULT_VALUE_ATT = "default_value";
const QString FENICS_FUNCTION_PARAMETER_IS_OPTIONAL_ATT = "is_optional";

const QString FENICS_FUNCTION_COMBO_VALUES_TAG = "combo-values";
const QString FENICS_FUNCTION_COMBO_VALUE_TAG = "value";
const QString FENICS_FUNCTION_COMBO_VALUE_NAME_ATT = "name";
const QString FENICS_FUNCTION_COMBO_VALUE_FENICS_VALUE_ATT = "fenics_value";


// BUILD SOURCE
const QString FENICS_LIBRARY_NAME = "Library";
const QString FENICS_MESH_NAME = "Mesh";
const QString FENICS_FUNCTION_SPACE_NAME = "Function Spaces";
const QString FENICS_USER_MESH_NAME= "Select your mesh file";



const QString FENICS_FILE_MESH_VAR_NAME = "mesh";

#endif // FENICSDEFINES_H
