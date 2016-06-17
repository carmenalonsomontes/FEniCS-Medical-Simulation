#ifndef FENICSPARAMETERPIPELINEDATA_H
#define FENICSPARAMETERPIPELINEDATA_H

#include <QString>
class FEniCSParameterPipelineData
{
public:
    FEniCSParameterPipelineData();



    QString getParameterName();
    QString getMethodName();
    QString getDescription();
    QString getSelectedValue();
    QString getDefaultValue();
    QString getComboValueSelected();
    QString getCombo_fenics_value();



    void setParameterName(QString val);
    void setMethodName(QString val);
    void setDescription(QString val);
    void setSelectedValue(QString val);
    void setDefaultValue(QString val);
    void sgetComboValueSelected(QString val);
    void setCombo_fenics_value(QString val);

    void clear();


private:
    QString _parameterName;
    QString _methodName;
    QString _description;
    QString _selectedValue;
    QString _defaultValue;
    QString _comboValueSelected;
    QString _combo_fenics_value;
};

#endif // FENICSPARAMETERPIPELINEDATA_H
