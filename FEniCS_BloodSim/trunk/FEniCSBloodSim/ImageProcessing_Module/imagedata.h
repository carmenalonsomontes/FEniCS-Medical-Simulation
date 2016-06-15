#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <QString>
#include <vtkVolume.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>

class ImageData
{
public:
    ImageData();
    ~ImageData();

    void loadImageData(QString imPath);

    vtkSmartPointer<vtkVolume> getVolumeData();
    void setVolumeData(vtkSmartPointer<vtkVolume> volPointer);

    vtkSmartPointer<vtkImageData> getImageData();
    void setImageData(vtkSmartPointer<vtkImageData> imData);

    bool isEmptyData();

    bool isVolume3D();
    bool setIsVolume3D(bool _val);

    void clear();


private:

    vtkSmartPointer<vtkImageData> _imageData;
    vtkSmartPointer<vtkVolume> _imVolumeData;
    bool _isEmpty;
    bool _is3D;


};

#endif // IMAGEDATA_H
