#include "imagedata.h"

#include "ImageProcessing_Module/imagereader.h"
ImageData::ImageData()
{
    //  _imData3D = ImageType3D::New();
      _imVolumeData = vtkVolume::New();
      _imageData = vtkImageData::New();
}

/*
ImageType3D::ConstPointer ImageData::getImData3D()
{
    return _imData3D;
}

void ImageData::setImData3D(ImageType3D::ConstPointer imPointer)
{
    _imData3D = imPointer;
}
*/


vtkVolume * ImageData::getVolumeData()
{
    return _imVolumeData;
}

void ImageData::setVolumeData(vtkVolume * volPointer)
{
    _imVolumeData = volPointer;
}


vtkImageData * ImageData::getImageData()
{
    return _imageData;
}

void ImageData::setImageData(vtkImageData * imData)
{
    _imageData = imData;
}


void ImageData::loadImageData(QString imPath)
{
    ImageReader _imReader;
    _imReader.readImage(this,imPath);

}
