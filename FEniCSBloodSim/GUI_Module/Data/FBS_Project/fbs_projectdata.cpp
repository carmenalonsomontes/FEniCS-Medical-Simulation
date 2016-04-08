#include "fbs_projectdata.h"

#include <QFileInfo>

#include "GUI_Module/XML/xmlprojectdata.h"
FBS_ProjectData::FBS_ProjectData()
{
    initializeVariables();
}

FBS_ProjectData::~FBS_ProjectData()
{
    delete _imageData;
}

void FBS_ProjectData::initializeVariables()
{
    _projectName = "";
    _projectPath = "";
    _imPath = "";
    _imName = "";
    _imPrefixSeries = "";

    _imageData = new ImageData();
}

QString FBS_ProjectData::getProjectPath()
{
    return _projectPath;
}

QString FBS_ProjectData::getProjectName()
{
    return _projectName;
}

QString FBS_ProjectData::getImPath()
{
    return _imPath;
}

QString FBS_ProjectData::getImPrefixSeries()
{
    return _imPrefixSeries;
}

QString FBS_ProjectData::getImName()
{
    return _imName;
}

void FBS_ProjectData::setImageData(ImageData * data)
{
    _imageData = data;
}
ImageData * FBS_ProjectData::getImageData()
{
    return _imageData;
}

void FBS_ProjectData::setProjectPath(QString _path)
{
    _projectPath = _path;
}

void FBS_ProjectData::setProjectName(QString _name)
{
    _projectName = _name;
}
void FBS_ProjectData::setImPath(QString _path)
{
    _imPath =_path;
}

void FBS_ProjectData::setImPrefixSeries(QString _prefixSeries)
{
    _imPrefixSeries = _prefixSeries;
}

void FBS_ProjectData::setImName(QString _name)
{
    _imName = _name;
}

void FBS_ProjectData::saveProjectPathName(QString _projectPathName)
{
    setProjectPath(QFileInfo(_projectPathName).absolutePath());
    setProjectName(QFileInfo(_projectPathName).baseName());
}

void FBS_ProjectData::saveProjectInfoToFile()
{
    XMLProjectData _xmlWriter;

    _xmlWriter.saveProjectDataFile(this);
}


void FBS_ProjectData::readProjectInfoFromFile(QString _projectFilePath)
{
    XMLProjectData _xmlReader;
    _xmlReader.readProjectDataFile(this,_projectFilePath);
}


void FBS_ProjectData::clearData()
{
  initializeVariables();
}

bool FBS_ProjectData::isEmpty()
{
    if ((!_projectName.isEmpty()) || (!_projectPath.isEmpty()))
        return false;
    return true;
}
bool FBS_ProjectData::isEmptyImagingData()
{
    if (!_imName.isEmpty() || (!_imPath.isEmpty()) || (!_imPrefixSeries.isEmpty()))
            return false;
    return true;
}


void FBS_ProjectData::loadImData()
{
    if (_imPath.isEmpty())
        return;
    _imageData->loadImageData(_imPath);
}


