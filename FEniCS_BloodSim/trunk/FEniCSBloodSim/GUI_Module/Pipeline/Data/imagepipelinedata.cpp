#include "imagepipelinedata.h"

ImagePipelineData::ImagePipelineData()
{
}


void ImagePipelineData::addImage2Pipeline(ImageType2D::Pointer _image)
{
    _imagesPipeline.append(_image);
}

ImageType2D::Pointer ImagePipelineData::getImage(int posPipeline)
{
    if (posPipeline >= _imagesPipeline.size())
        return NULL;
    return _imagesPipeline.at(posPipeline);
}
