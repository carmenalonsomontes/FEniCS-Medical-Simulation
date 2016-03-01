#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H

#include "Defines/GeneralImageDefines.h"

#include "Data/angiogramvo.h"

class ImageHelper
{
public:
    ImageHelper();
    int readAngiogram(AngiogramVO * _angiogramVO);
    int writeSlice(AngiogramVO * _angiogramVO, string _sliceName, int _sliceNo);
    int writeSlices(AngiogramVO * _angiogramVO,string _slicePath, string _fileExtension);

};

#endif // IMAGEHELPER_H
