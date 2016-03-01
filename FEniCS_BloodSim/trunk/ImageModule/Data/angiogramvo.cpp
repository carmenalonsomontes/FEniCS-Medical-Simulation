#include "angiogramvo.h"

AngiogramVO::AngiogramVO()
{
}

// This is the name of the patient or ID --> we can use the name of the file too.
string AngiogramVO::getAngiogramFileName()
{
    return _angiogramFileName;
}

void AngiogramVO::setAngiogramFileName(string _filename)
{
    _angiogramFileName = _filename;
}

string AngiogramVO::getAngiogramTypeImFile()
{
    return _angiogramTypeImFile;
}

void AngiogramVO::setAngiogramTypeImFile(string _imType)
{
    _angiogramTypeImFile = _imType;
}

// this is the full path + filename
string AngiogramVO::getAngiogramPathImFile()
{
    return _angiogramPathImFile;
}

void AngiogramVO::setAngiogramPathImFile(string _imPath)
{
     _angiogramPathImFile = _imPath;
}

ImageType3D::ConstPointer AngiogramVO::getAngiogramDataIm3D()
{
    return _angiogramDataIm3D;
}

void AngiogramVO::setAngiogramDataIm3D(ImageType3D::ConstPointer imData)
{
    _angiogramDataIm3D = imData;
}

ImageType2D::Pointer AngiogramVO::getSlice()
{

    // TODO
     ImageType2D::Pointer outputImage = ImageType2D::New();
     return outputImage;

}
