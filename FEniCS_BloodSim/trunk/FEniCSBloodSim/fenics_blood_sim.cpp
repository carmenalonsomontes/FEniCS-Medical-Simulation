#include "fenics_blood_sim.h"
#include "ui_fenics_blood_sim.h"

#include <QDir>
#include <QAction>


FEniCS_Blood_Sim::FEniCS_Blood_Sim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FEniCS_Blood_Sim)
{
    ui->setupUi(this);
    // Data
    _projectData = new FBS_ProjectData();
    _userSessionData = new UserSessionData();
    // FileMenu
    _fileMenuBuilder = new FileMenuBuilder();
    _fileMenuBuilder->registerWindow(this);
    _fileMenuBuilder->registerProjectData(_projectData);

    connectSignalsMenuBuilder();
    loadRecentProjectList();
}


FEniCS_Blood_Sim::~FEniCS_Blood_Sim()
{
    delete ui;
    delete _fileMenuBuilder;
    delete _projectData;
    delete _userSessionData;
}

void FEniCS_Blood_Sim::connectSignalsMenuBuilder()
{
     connect(_fileMenuBuilder, SIGNAL(updateStatusBarUI(const QString)), this, SLOT(UpdateStatusBar(const QString)));
     connect(_fileMenuBuilder, SIGNAL(enableCloseProjectUI(bool)), this, SLOT(EnableCloseProjectUI(bool)));
     connect(_fileMenuBuilder, SIGNAL(enableSaveProjectUI(bool)), this, SLOT(EnableSaveProjectUI(bool)));
     connect(_fileMenuBuilder, SIGNAL(exittUI()), this, SLOT(exitApplicationUI()));
     connect(_fileMenuBuilder, SIGNAL(updateRecentProjectList(const QString)), this, SLOT(updateRecentProjectListUI(const QString)));

}

void FEniCS_Blood_Sim::loadRecentProjectList()
{
    QString _path = QDir(QDir::currentPath()).filePath(PREFERENCES_FILE_NAME);
    _userSessionData->setFileNamePath(_path);
    _userSessionData->loadRecentProjectList();
    if (_userSessionData->isEmpty())
        _userSessionData->saveRecentProjectList();
    else
        fillRecentProjectsSection();

}

// -------------------------------------------------
// FILE MENU


void FEniCS_Blood_Sim::on_actionNew_Project_triggered()
{
    _fileMenuBuilder->launchMenuAction(NEW_PROJECT);


}

void FEniCS_Blood_Sim::on_actionOpen_Project_triggered()
{
    _fileMenuBuilder->launchMenuAction(OPEN_PROJECT);


}

void FEniCS_Blood_Sim::on_actionSave_triggered()
{
    _fileMenuBuilder->launchMenuAction(SAVE_PROJECT);

}

void FEniCS_Blood_Sim::on_actionSave_As_triggered()
{
    _fileMenuBuilder->launchMenuAction(SAVEAS_PROJECT);

}

void FEniCS_Blood_Sim::on_actionClose_Project_triggered()
{
    _fileMenuBuilder->launchMenuAction(CLOSE_PROJECT);


}

void FEniCS_Blood_Sim::on_actionExit_triggered()
{
   closeApplication();
}

void FEniCS_Blood_Sim::on_actionExit_MainWindow_triggered()
{
  closeApplication();

}

void FEniCS_Blood_Sim::on_actionClear_list_triggered()
{

    if (_userSessionData->isEmpty()) return;
    if (ui->menuRecent_Projects->isEmpty()) return;

    QList<QAction *> _listActions = ui->menuRecent_Projects->actions();
    if (_listActions.isEmpty()) return;

    for (int i=0;i<_listActions.size();i++)
    {
        QAction * _project2Remove = _listActions.at(i);
        QString _projectName = _project2Remove->objectName();
        if ((_projectName != ui->action_recent_project_list->objectName()) &&
            (_projectName != ui->actionClear_list->objectName()))
            ui->menuRecent_Projects->removeAction(_project2Remove);
    }
    ui->action_recent_project_list->setVisible(true);
   _userSessionData->resetProjects();
}


void FEniCS_Blood_Sim::fillRecentProjectsSection()
{
    QStringList _projectList = _userSessionData->getRecentProjectList();
    if (_projectList.isEmpty())
        return;
    if (ui->action_recent_project_list->isVisible())
        ui->action_recent_project_list->setVisible(false);
    for (int i = 0; i< _projectList.size();i++)
    {
         QAction * _newRecentProjectAction = createRecentProjectAction(_projectList.at(i));
         ui->menuRecent_Projects->addAction(_newRecentProjectAction);
    }
}

QAction *  FEniCS_Blood_Sim::createRecentProjectAction(QString _name)
{
    QAction * _newRecentProjectAction = new QAction(this);
    _newRecentProjectAction->setText(_name );
    _newRecentProjectAction->setData(_name);
    _newRecentProjectAction->setVisible(true);
    connect(_newRecentProjectAction, SIGNAL(triggered()),this, SLOT(openRecentFile()));
    return _newRecentProjectAction;
}


void FEniCS_Blood_Sim::openRecentFile()
{
    _fileMenuBuilder->launchMenuAction(CLOSE_PROJECT);
    QString _file;
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        _file = action->data().toString();
    if (_file.isEmpty())
        return;

    _fileMenuBuilder->launchMenuAction(OPEN_PROJECT,_file);

}




//**************************************************************
void FEniCS_Blood_Sim::closeApplication()
{
    _projectData->saveProjectInfoToFile();
    _userSessionData->saveRecentProjectList();
    close();

}

void FEniCS_Blood_Sim::updateStatusBar()
{
    QString _message = "Project: " + _projectData->getProjectName();
    statusBar()->showMessage(_message);
}


void FEniCS_Blood_Sim::UpdateStatusBar(const QString text)
{
    statusBar()->showMessage(text);
}
void FEniCS_Blood_Sim::EnableCloseProjectUI(bool val)
{
    ui->actionClose_Project->setEnabled(val);
}
void FEniCS_Blood_Sim::EnableSaveProjectUI(bool val)
{
    ui->actionSave->setEnabled(val);
    ui->actionSave_As->setEnabled(val);
 }

void FEniCS_Blood_Sim::updateRecentProjectListUI(const QString projectPath)
{
    bool _inserted = _userSessionData->updateRecentProjectList(projectPath);
    if (_inserted)
    {
        QAction * _newRecentProjectAction = createRecentProjectAction(projectPath);
        ui->menuRecent_Projects->addAction(_newRecentProjectAction);
    }
}