#ifndef FENICS_BLOOD_SIM_H
#define FENICS_BLOOD_SIM_H

#include <QMainWindow>

#include "GUI_Module/Menu/filemenubuilder.h"
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "GUI_Module/Data/FBS_Project/fbs_projectdata.h"
#include "GUI_Module/Session/usersessiondata.h"

namespace Ui {
class FEniCS_Blood_Sim;
}

class FEniCS_Blood_Sim : public QMainWindow
{
    Q_OBJECT

public:
    explicit FEniCS_Blood_Sim(QWidget *parent = 0);
    ~FEniCS_Blood_Sim();

     void updateStatusBar();


private slots:
    // EDIT
    void on_actionNew_Project_triggered();

    void on_actionOpen_Project_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionClose_Project_triggered();

    void on_actionExit_triggered();

    void on_actionExit_MainWindow_triggered();


    void UpdateStatusBar(const QString text);
    void EnableCloseProjectUI(bool val);
    void EnableSaveProjectUI(bool val);

    void on_actionClear_list_triggered();

    void updateRecentProjectListUI(const QString projectPath);
    void openRecentFile();
private:
    Ui::FEniCS_Blood_Sim *ui;
    FileMenuBuilder *  _fileMenuBuilder;


    // Data
    FBS_ProjectData * _projectData;
    UserSessionData * _userSessionData;


    void connectSignalsMenuBuilder();
    void loadRecentProjectList();
    void closeApplication();

    void fillRecentProjectsSection();
    QAction *  createRecentProjectAction(QString _name);
};

#endif // FENICS_BLOOD_SIM_H



