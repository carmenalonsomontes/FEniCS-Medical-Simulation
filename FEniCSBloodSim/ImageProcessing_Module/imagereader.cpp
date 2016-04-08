#include "imagereader.h"


// To read  & Write images in 2D and 3D

ImageReader::ImageReader()
{
}

void ImageReader::readImage(ImageData * imageData,QString imPath)
{
    if ((imPath.isEmpty()) || (imPath.isNull()))
        return;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(imPath.toStdString());
    reader->Update();
    imageData->setImData3D(reader->GetOutput());
}

