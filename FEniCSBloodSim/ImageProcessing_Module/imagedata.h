#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "ImageProcessing_Module/Defines/ImageDefines.h"
#include <QString>

class ImageData
{
public:
    ImageData();

    void loadImageData(QString imPath);

    ImageType3D::ConstPointer getImData3D();
    void setImData3D(ImageType3D::ConstPointer imPointer);

private:

    ImageType3D::ConstPointer _imData3D;

};

#endif // IMAGEDATA_H
