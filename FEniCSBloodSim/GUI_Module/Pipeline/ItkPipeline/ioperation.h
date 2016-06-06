#ifndef IOPERATION_H
#define IOPERATION_H

#include <QStringList>
#include <iostream>
#include "itkImage.h"

using namespace std;
typedef itk::Image<unsigned char, 2>  ImageType;

class IOperation
{
public:

    //IOperation();
    virtual void SetInPut(ImageType::Pointer _input) = 0;
    virtual void SetParameters(QStringList _parameterList) = 0;
    virtual ImageType::Pointer GetOutput() = 0;

};

#endif // IOPERATION_H
