#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "ImageProcessing_Module/Defines/ImageDefines.h"
#include <QString>
#include <vtkVolume.h>
#include <vtkImageData.h>
class ImageData
{
public:
    ImageData();

    void loadImageData(QString imPath);

  /*  ImageType3D::ConstPointer getImData3D();
    void setImData3D(ImageType3D::ConstPointer imPointer);
 */
    vtkVolume * getVolumeData();
    void setVolumeData(vtkVolume * volPointer);

    vtkImageData * getImageData();
    void setImageData(vtkImageData * imData);

private:

   // ImageType3D::ConstPointer _imData3D;
    vtkImageData * _imageData;
    vtkVolume * _imVolumeData;

};

#endif // IMAGEDATA_H
