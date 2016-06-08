#ifndef IMAGEPIPELINEDATA_H
#define IMAGEPIPELINEDATA_H

#include <QList>
#include "GUI_Module/Pipeline/ItkPipeline/defines/OperationDefines.h"
#include "itkImage.h"

//using namespace std;

//typedef itk::Image<unsigned char, 2>  ImageType;

class ImagePipelineData
{
public:
    ImagePipelineData();
    void addImage2Pipeline(ImageType2D::Pointer _image);
    ImageType2D::Pointer getImage(int posPipeline);

private:
    QList<ImageType2D::Pointer> _imagesPipeline;
};

#endif // IMAGEPIPELINEDATA_H
