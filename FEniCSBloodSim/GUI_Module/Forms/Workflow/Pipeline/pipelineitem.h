#ifndef PIPELINEITEM_H
#define PIPELINEITEM_H

#include <QString>
#include <QList>

#include "configurationpipelineitem.h"
#include "GUI_Module/Pipeline/ItkPipeline/defines/OperationDefines.h"

#include "ImageProcessing_Module/imagedata.h"

class PipelineItem
{
public:
    PipelineItem();
    ~PipelineItem();

    QString getIconPath();
    QString getDescription();
    int getPositionPipeline();



    void setIconPath(QString newPath);
    void setDescription(QString newDesc);
    void setPositionPipeline(int newPos);

    QString getCategoryName();
    void setCategoryName(QString newCatName);


    QList<ConfigurationPipelineItem> getConfigurationItemList();
    void  setConfigurationItemList(QList<ConfigurationPipelineItem> list);


    QString getFunctionName();
    QString getFunctionClassName();
    QString getFunctionDescription();

    void setFunctionName(QString name);
    void setFunctionClassName(QString className);
    void setFunctionDescription(QString  desc);


    void updateConfiguredItemValue(QString _itemValue, int pos);

    bool isEmpty();
    bool isImage3DEmpty();
   // bool isImage2DEmpty();
    void clear();

    ImageType3D::Pointer getImage3D();
    QString getImage3DPath();
    void setImage3DPath(QString path);

    void setImage3D( ImageType3D::Pointer _image);
   // void setImage3D( ImageType3D::Pointer _image, QString path);


   // ImageType2D::Pointer getImage2D();
   // void setImage2D( ImageType2D::Pointer _image);

    // VTK
    ImageData  * getImageData();
    bool isImageLoaded();

    void loadImage();


private:
    QString _categoryName;
    QString _iconPath;
    QString _description;

    QString _functionName;
    QString _functionClassName;
    QString _functionDescription;

    QList<ConfigurationPipelineItem> _configurationItemList;
    int _positionPipeline;



    // Results of the Pipeline execution
    ImageType3D::Pointer _image3D;
    QString _image3DPath;
   // ImageType2D::Pointer _image2D;

    // VTK Images
    ImageData  * _imageData;
    bool _isImageDataLoaded;
    //vtkImageData * _imageData;




};

#endif // PIPELINEITEM_H
