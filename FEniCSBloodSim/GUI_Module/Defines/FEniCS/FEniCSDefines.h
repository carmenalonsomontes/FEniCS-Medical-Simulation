#ifndef FENICSDEFINES_H
#define FENICSDEFINES_H

#include <QString>


enum FENICSDialog{
    FENICS_METHOD_NAME_COLUMN = 0,
    FENICS_ACTION_COLUMN = 1
};



const QString FENICS_FILE_PATH=":/files/GUI_Module/Forms/FEniCS/XML/FEniCSXmlCodeSnippsets.xml";


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

#endif // FENICSDEFINES_H
