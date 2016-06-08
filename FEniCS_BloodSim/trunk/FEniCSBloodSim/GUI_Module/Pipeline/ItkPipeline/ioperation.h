#ifndef IOPERATION_H
#define IOPERATION_H

#include <QString>
#include <QStringList>

#include "defines/OperationDefines.h"
#include <iostream>



class IOperation
{
public:

    //IOperation();
    virtual void SetInPut(ImageType2D::Pointer _input) = 0;
    virtual void SetInPut(ImageType3D::Pointer _input) = 0;

    virtual void SetParameters(QStringList _parameterList) = 0;

    virtual ImageType2D::Pointer GetOutput2D() = 0;
    virtual ImageType3D::Pointer GetOutput3D() = 0;

    virtual void set3D(bool _val) = 0;

    virtual bool is3D() = 0;

    virtual void exec() = 0;
    virtual void save(QString _path) = 0;


};

#endif // IOPERATION_H
