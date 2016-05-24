#ifndef IMAGINGWKFFUNCTIONDATA_H
#define IMAGINGWKFFUNCTIONDATA_H

#include <QString>
#include <QStringList>

class ImagingWkfFunctionData
{
public:
    ImagingWkfFunctionData();

    QString getName();
    QString getClassName();
    QString getDescription();
    //QString getParameters();


    void setName(QString newName);
    void setClassName(QString newClassname);
    void setDescription(QString newDescription);
    //void setParameters(QString newParameters);


    QStringList getParameterClassName();
    QStringList getParametersName();
    QStringList getParameterType();

    void setParameterClassName(QStringList list);
    void setParametersName(QStringList list);
    void setParameterType(QStringList list);


    QStringList getDefaultValue();
    void  setDefaultValue(QStringList newList);

    void clear();
    bool isEmpty();

private:
    QString _name;
    QString _className;
    QString _description;
    QStringList _parameterClassName;
    QStringList _parametersName;
    QStringList _parameterType;
    QStringList _defaultValue;
};

#endif // IMAGINGWKFFUNCTIONDATA_H
