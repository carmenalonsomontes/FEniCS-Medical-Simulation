#ifndef IMAGINGWKFFUNCTIONDATA_H
#define IMAGINGWKFFUNCTIONDATA_H

#include <QString>

class ImagingWkfFunctionData
{
public:
    ImagingWkfFunctionData();

    QString getName();
    QString getClassName();
    QString getDescription();
    QString getParameters();


    void setName(QString newName);
    void setClassName(QString newClassname);
    void setDescription(QString newDescription);
    void setParameters(QString newParameters);

    void clear();
    bool isEmpty();

private:
    QString _name;
    QString _className;
    QString _description;
    QString _parameters;
};

#endif // IMAGINGWKFFUNCTIONDATA_H
