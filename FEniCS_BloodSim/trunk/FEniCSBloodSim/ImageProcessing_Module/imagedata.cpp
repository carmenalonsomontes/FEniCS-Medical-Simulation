#include "imagedata.h"

#include "ImageProcessing_Module/imagereader.h"
ImageData::ImageData()
{
      _imVolumeData = vtkSmartPointer<vtkVolume>::New();
      _imageData = vtkSmartPointer<vtkImageData>::New();
      _isEmpty = true;
      _is3D = true; // By default this is considered for 3D volumes

}
ImageData::~ImageData()
{
    clear();
}

vtkSmartPointer<vtkVolume> ImageData::getVolumeData()
{
    return _imVolumeData;
}

void ImageData::setVolumeData(vtkSmartPointer<vtkVolume> volPointer)
{
    _imVolumeData = volPointer;
    _isEmpty = false;
}


vtkSmartPointer<vtkImageData>  ImageData::getImageData()
{
    return _imageData;
}

void ImageData::setImageData(vtkSmartPointer<vtkImageData> imData)
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

bool ImageData::isVolume3D()
{
    return _is3D;
}

bool ImageData::setIsVolume3D(bool _val)
{
 _is3D = _val;
}



void ImageData::clear()
{
    _imVolumeData = NULL;
    _imageData = NULL;
}

bool ImageData::isEmptyData()
{
    return _isEmpty;
}
