#ifndef XMLUSERSESSIONDATA_H
#define XMLUSERSESSIONDATA_H

#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "GUI_Module/Session/usersessiondata.h"

class XMLUserSessionData
{
public:
    XMLUserSessionData();
    bool saveUserSessionData(UserSessionData * _userSessionData, QString path);
    void readUserSessionData(UserSessionData * _userSessionData, QString path);

private:

    bool createNewFile(QFile * _file,UserSessionData * _userSessionData);
    void writeRecentProjectList(QXmlStreamWriter * xmlWriter, UserSessionData * _userSessionData);
    void readRecentProjectList( QXmlStreamReader * xmlReader,UserSessionData * _userSessionData);
};

#endif // XMLUSERSESSIONDATA_H
