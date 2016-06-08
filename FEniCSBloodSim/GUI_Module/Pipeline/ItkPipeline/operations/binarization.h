#ifndef BINARIZATION_H
#define BINARIZATION_H

#include "../ioperation.h"
#include <QStringList>
#include "itkBinaryThresholdImageFilter.h"

typedef BinaryThresholdImageFilter <ImageType2D, ImageType2D>  BinaryThresholdImageFilterType2D;
typedef BinaryThresholdImageFilter <ImageType3D, ImageType3D>  BinaryThresholdImageFilterType3D;

class Binarization : public IOperation
{
public:
    Binarization();

    void SetInPut(ImageType2D::Pointer _input);
    void SetInPut(ImageType3D::Pointer _input);

    void SetParameters(QStringList _parameterList);

    static IOperation * Create();

    ImageType2D::Pointer GetOutput2D();
    ImageType3D::Pointer GetOutput3D();

    void set3D(bool _val);
    bool is3D();

    void exec();
    void save(QString _path);


private:
    BinaryThresholdImageFilterType2D::Pointer thresholdFilter2D;
    BinaryThresholdImageFilterType3D::Pointer thresholdFilter3D;
    bool _is3D;

    void setParameters3D(QStringList _parameterList);
    void setParameters2D(QStringList _parameterList);

    void save3D(QString _path);
    void save2D(QString _path);

};

#endif // BINARIZATION_H
