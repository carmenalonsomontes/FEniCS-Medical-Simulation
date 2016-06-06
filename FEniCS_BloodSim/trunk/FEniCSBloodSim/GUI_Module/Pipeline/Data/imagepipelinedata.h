#ifndef IMAGEPIPELINEDATA_H
#define IMAGEPIPELINEDATA_H

#include <QList>

#include "itkImage.h"

using namespace std;

typedef itk::Image<unsigned char, 2>  ImageType;

class ImagePipelineData
{
public:
    ImagePipelineData();
    void addImage2Pipeline(ImageType::Pointer _image);
    ImageType::Pointer getImage(int posPipeline);

private:
    QList<ImageType::Pointer> _imagesPipeline;
};

#endif // IMAGEPIPELINEDATA_H
