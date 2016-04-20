#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "ImageProcessing_Module/Defines/ImageDefines.h"
#include <QString>
#include <vtkVolume.h>
#include <vtkImageData.h>
#include <vtkImageViewer2.h>
#include <vtkAlgorithmOutput.h>

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

    vtkImageViewer2 * getImageViewer();
    void setImageViewer(vtkAlgorithmOutput * outputPort);


    void setAlgorithmOutput(vtkAlgorithmOutput * output);
    vtkAlgorithmOutput * getAlgorithmOutput();


private:

   // ImageType3D::ConstPointer _imData3D;
    vtkImageData * _imageData;
    vtkVolume * _imVolumeData;
    vtkImageViewer2 * _imViewer;
    vtkAlgorithmOutput * _outputConnect;

};

#endif // IMAGEDATA_H
