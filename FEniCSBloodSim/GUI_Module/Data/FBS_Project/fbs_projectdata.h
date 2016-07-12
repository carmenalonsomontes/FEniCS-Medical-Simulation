#ifndef FBS_PROJECTDATA_H
#define FBS_PROJECTDATA_H

#include <QString>
#include <QList>

#include "ImageProcessing_Module/imagedata.h"
#include "GUI_Module/Forms/Workflow/Pipeline/pipelineitem.h"


class FBS_ProjectData
{
public:
    FBS_ProjectData();
    ~FBS_ProjectData();

    QString getProjectPath();
    QString getProjectName();

    QString getImPath();
    QString getImPrefixSeries();
    QString getImName();

    void setImageData(ImageData * data);
    ImageData * getImageData();
    void loadImData();


    void setProjectPath(QString _path);
    void setProjectName(QString _name);
    void setImPath(QString _path);
    void setImPrefixSeries(QString _prefixSeries);
    void setImName(QString _name);

    void saveProjectPathName(QString _projectPathName);
    void saveProjectInfoToFile();
    void readProjectInfoFromFile(QString _projectFilePath);

    QList<PipelineItem> getListPipelineItems();
    void setListPipelineItems( QList<PipelineItem> _list);


    QString getFenicsSimFileName();
    QString getFenicsSimPath();

    void setFenicsSimFileName(QString name);
    void setFenicsSimPath(QString path);

    void clearData();

    bool isEmpty();
    bool isEmptyImagingData();
    bool isEmptyPipelineData();

private:
    QString _projectPath;
    QString _projectName;

    QString _imPath;
    QString _imPrefixSeries;
    QString _imName;

    QString _fenicsSimFileName;
    QString _fenicsSimPath;

    ImageData * _imageData;

    QList<PipelineItem> _listPipelineItems;

    void initializeVariables();


};

#endif // FBS_PROJECTDATA_H
