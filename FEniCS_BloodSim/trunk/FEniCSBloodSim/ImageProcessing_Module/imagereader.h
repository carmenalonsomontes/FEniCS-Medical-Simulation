#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <QString>

#include "ImageProcessing_Module/Defines/ImageDefines.h"
#include "ImageProcessing_Module/imagedata.h"

class ImageReader
{
public:
    ImageReader();

    void readImage(ImageData * imageData,QString path);


};

#endif // IMAGEREADER_H
