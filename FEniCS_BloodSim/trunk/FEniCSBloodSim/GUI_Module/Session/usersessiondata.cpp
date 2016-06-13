#include "usersessiondata.h"

#include "../XML/xmlusersessiondata.h"
#include <QFileInfo>

UserSessionData::UserSessionData()
{

}
void UserSessionData::loadAllData()
{
    XMLUserSessionData _xmlReader;
    _xmlReader.readUserSessionData(this,_filePath);
}

void UserSessionData::saveAllData()
{
    XMLUserSessionData _xmlWriter;
    _xmlWriter.saveUserSessionData(this,_filePath);
}


/*void UserSessionData::loadRecentProjectList()
{
    XMLUserSessionData _xmlReader;
    _xmlReader.readUserSessionData(this,_filePath);
}
void UserSessionData::saveRecentProjectList()
{
     XMLUserSessionData _xmlWriter;
     _xmlWriter.saveUserSessionData(this,_filePath);
}*/

bool UserSessionData::updateRecentProjectList(QString _projectPath)
{
     // Check if already exists
     if (_recentProjectList.contains(_projectPath))
         return false;

     // Add to the list if it does not exist
    QString _nameProject = QFileInfo(_projectPath).baseName();
    _recentProjectList.append(_projectPath);
    _recentProjectNameList.append(_nameProject);
    return true;
}

QStringList UserSessionData::getRecentProjectList()
{
    return _recentProjectList;
}

QStringList UserSessionData::getRecentProjectNameList()
{
    return _recentProjectNameList;
}

bool UserSessionData::isEmptyRecentProjects()
{
    return _recentProjectList.isEmpty();
}

void UserSessionData::setFileNamePath(QString _path)
{
    _filePath = _path;
}
void UserSessionData::resetProjects()
{
    _recentProjectList.clear();
    _recentProjectNameList.clear();
}


QString UserSessionData::getMeshToolPath()
{
    return _meshToolPath ;
}

void UserSessionData::setMeshToolPath(QString toolpath)
{
    _meshToolPath = toolpath;
}

QString UserSessionData::getFenicsToolPath()
{
    return _fenicsToolPath;
}

void UserSessionData::setFenicsToolPath(QString toolpath)
{
    _fenicsToolPath = toolpath;
}

// IMAGE TABS

void UserSessionData::setImageTabsInfo(int noImageRow, ImageTabs sliceType, int noSlice, int maxSlice, int minSlice)
{
    bool _found = false;
    for (int i=0; (i< _imagesTabInfo.size()) && (!_found) ; i++)
    {
        UserImageInfoSession _tmpVal = _imagesTabInfo.at(i);
        if( (_tmpVal.getNoImageRow() == noImageRow) && (_tmpVal.getSliceType() == sliceType))
        {
            _tmpVal.setMaxSlice(maxSlice);
            _tmpVal.setMinSlice(minSlice);
            _tmpVal.setNoSlice(noSlice);
            _imagesTabInfo.replace(i,_tmpVal);
            _found = true;
        }

    }

    if (!_found)
    {
        UserImageInfoSession _newVal;
        _newVal.setNoImageRow(noImageRow);
        _newVal.setSliceType(sliceType);
        _newVal.setNoSlice(noSlice);
        _newVal.setMaxSlice(maxSlice);
        _newVal.setMinSlice(minSlice);
        _imagesTabInfo.append(_newVal);

    }


}

UserImageInfoSession UserSessionData::getImageTabsInfo(int noImageRow, ImageTabs sliceType )
{

    UserImageInfoSession _result;
    for (int i=0; i< _imagesTabInfo.size() ; i++)
    {
        UserImageInfoSession _tmpVal = _imagesTabInfo.at(i);
        if( (_tmpVal.getNoImageRow() == noImageRow) && (_tmpVal.getSliceType() == sliceType))
            return _tmpVal;
    }

    return _result;
}

bool UserSessionData::existImageInfo(int noImageRow, ImageTabs sliceType )
{
    bool _found = false;

    for (int i=0; ( i< _imagesTabInfo.size()) && (!_found) ; i++)
    {
        UserImageInfoSession _tmpVal = _imagesTabInfo.at(i);
        if( (_tmpVal.getNoImageRow() == noImageRow) && (_tmpVal.getSliceType() == sliceType))
            _found = true;
    }
    return _found;
}

void UserSessionData::clearImageTabsInfo()
{
    _imagesTabInfo.clear();
}
bool UserSessionData::isEmptyImageTabInfo()
{
    return _imagesTabInfo.isEmpty();
}
