#include "imagedata.h"

#include "ImageProcessing_Module/imagereader.h"
ImageData::ImageData()
{
      _imVolumeData = vtkVolume::New();
      _imageData = vtkImageData::New();
      _isEmpty = true;

}


vtkVolume * ImageData::getVolumeData()
{
    return _imVolumeData;
}

void ImageData::setVolumeData(vtkVolume * volPointer)
{
    _imVolumeData = volPointer;
    _isEmpty = false;
}


vtkImageData * ImageData::getImageData()
{
    return _imageData;
}

void ImageData::setImageData(vtkImageData * imData)
{
    _imageData = imData;
    _isEmpty = false;
}


void ImageData::loadImageData(QString imPath)
{
    ImageReader _imReader;
    _imReader.readImage(this,imPath);
    _isEmpty = false;
}

bool ImageData::isEmptyData()
{
    return _isEmpty;
}
