#ifndef BINARYTHRESHOLD2D_H
#define BINARYTHRESHOLD2D_H

#include "../itkpipelineoperation.h"

#include "itkImage.h"
#include "itkBinaryThresholdImageFilter.h"
#include <itkImageFileReader.h>
#include "QuickView.h"

#include "GUI_Module/ITK_PIPELINE/ItkPipelineImpl.h"


class BinaryThreshold2D: public ItkPipelineImpl<BinaryThreshold2D>
{

public:
    BinaryThreshold2D(): ItkPipelineImpl<BinaryThreshold2D>(){}
    static const std::string Name;

    /*BinaryThreshold2D();

    void exec();
    void SetParatemeters(QStringList parameters);


protected:
     void loadInput(QString _path);


private:

    typedef itk::Image<unsigned char, 2>  ImageType;
    typedef itk::ImageFileReader<ImageType> ReaderType;
     typedef itk::BinaryThresholdImageFilter <ImageType, ImageType>
        BinaryThresholdImageFilterType;
    BinaryThresholdImageFilterType::Pointer thresholdFilter;

*/

};

#endif // BINARYTHRESHOLD2D_H
