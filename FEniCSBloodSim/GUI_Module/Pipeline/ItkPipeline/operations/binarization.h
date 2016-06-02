#ifndef BINARIZATION_H
#define BINARIZATION_H


#include "../ioperation.h"
#include <QStringList>
#include "itkImage.h"
#include "itkBinaryThresholdImageFilter.h"
#include <itkImageFileReader.h>
#include "QuickView.h"
#include <itkSmartPointer.h>

using namespace std;
//typedef itk::Image<unsigned char, 2>  ImageType;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::BinaryThresholdImageFilter <ImageType, ImageType>
    BinaryThresholdImageFilterType;

//using namespace itk;
class Binarization : public IOperation
{
public:
    Binarization();
    void SetParameters(QStringList _parameterList);
    void execOperation();
    static IOperation * Create();
    ImageType::Pointer GetPointer(){ return thresholdFilter->GetOutput();}

private:

    BinaryThresholdImageFilterType::Pointer thresholdFilter;



};

#endif // BINARIZATION_H
