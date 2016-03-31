#include "usersessiondata.h"

#include "../XML/xmlusersessiondata.h"
#include <QFileInfo>

UserSessionData::UserSessionData()
{

}

 void UserSessionData::loadRecentProjectList()
 {
    XMLUserSessionData _xmlReader;
    _xmlReader.readUserSessionData(this,_filePath);
 }
 void UserSessionData::saveRecentProjectList()
 {
     XMLUserSessionData _xmlWriter;
     _xmlWriter.saveUserSessionData(this,_filePath);
 }

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

bool UserSessionData::isEmpty()
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
