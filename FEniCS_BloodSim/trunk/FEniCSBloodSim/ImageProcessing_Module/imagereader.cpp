#include "imagereader.h"

#include <QFileInfo>
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkSmartPointer.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkNIFTIImageReader.h>
#include <vtkSmartPointer.h>

// To read  & Write images in 2D and 3D

ImageReader::ImageReader()
{
}

void ImageReader::readImage(ImageData * imageData,QString imPath)
{
    if ((imPath.isEmpty()) || (imPath.isNull()))
        return;


    bool _isNIIFile = false;
    QString _fileSuffix = QFileInfo(imPath).completeSuffix();
    if (_fileSuffix.contains("nii"))
        _isNIIFile = true;

    // Create our volume and mapper
    vtkSmartPointer<vtkVolume>volume = vtkSmartPointer<vtkVolume>::New();
    vtkSmartPointer<vtkSmartVolumeMapper> mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    // Create the property and attach the transfer functions
    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();

    // Read file
    if (!_isNIIFile)
    {
        vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
        vtkSmartPointer<vtkImageReader2> reader = readerFactory->CreateImageReader2(imPath.toStdString().c_str());
        reader->SetFileName(imPath.toStdString().c_str());
        reader->Update();
        imageData->setImageData(reader->GetOutput());
        mapper->SetInputConnection( reader->GetOutputPort() );
    }else
    {
        vtkSmartPointer<vtkNIFTIImageReader> _niftiReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
        _niftiReader->SetFileName(imPath.toStdString().c_str());
        _niftiReader->Update();
        imageData->setImageData(_niftiReader->GetOutput());
        mapper->SetInputConnection( _niftiReader->GetOutputPort() );
    }
    property->SetInterpolationTypeToLinear();
    // connect up the volume to the property and the mapper
    volume->SetProperty( property );
    volume->SetMapper( mapper );
    mapper->SetBlendModeToMaximumIntensity();
    imageData->setVolumeData(volume);
    imageData->setIsVolume3D(true);


}

