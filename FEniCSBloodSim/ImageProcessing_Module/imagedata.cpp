#include "imagedata.h"

#include "ImageProcessing_Module/imagereader.h"
ImageData::ImageData()
{
      _imData3D = ImageType3D::New();
}


ImageType3D::ConstPointer ImageData::getImData3D()
{
    return _imData3D;
}

void ImageData::setImData3D(ImageType3D::ConstPointer imPointer)
{
    _imData3D = imPointer;
}


void ImageData::loadImageData(QString imPath)
{
    ImageReader _imReader;
    _imReader.readImage(this,imPath);

}
