#ifndef BINARIZATION_H
#define BINARIZATION_H


#include "../ioperation.h"
#include <QStringList>
#include "itkImage.h"
#include "itkBinaryThresholdImageFilter.h"
#include <itkImageFileReader.h>


using namespace std;
using namespace itk;
typedef ImageFileReader<ImageType> ReaderType;
typedef BinaryThresholdImageFilter <ImageType, ImageType>    BinaryThresholdImageFilterType;

class Binarization : public IOperation
{
public:
    Binarization();
    void SetParameters(QStringList _parameterList);

    static IOperation * Create();
    ImageType::Pointer GetOutput();

private:

    BinaryThresholdImageFilterType::Pointer thresholdFilter;



};

#endif // BINARIZATION_H
