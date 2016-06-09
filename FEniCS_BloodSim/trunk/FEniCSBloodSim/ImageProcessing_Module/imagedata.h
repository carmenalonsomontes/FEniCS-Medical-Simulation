#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <QString>
#include <vtkVolume.h>
#include <vtkImageData.h>

class ImageData
{
public:
    ImageData();

    void loadImageData(QString imPath);

    vtkVolume * getVolumeData();
    void setVolumeData(vtkVolume * volPointer);

    vtkImageData * getImageData();
    void setImageData(vtkImageData * imData);

    bool isEmptyData();

    bool isVolume3D();
    bool setIsVolume3D(bool _val);

    void clear();


private:

    vtkImageData * _imageData;
    vtkVolume * _imVolumeData;
    bool _isEmpty;
    bool _is3D;


};

#endif // IMAGEDATA_H
