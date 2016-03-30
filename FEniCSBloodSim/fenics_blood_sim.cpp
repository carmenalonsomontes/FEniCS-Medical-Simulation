#include "fenics_blood_sim.h"
#include "ui_fenics_blood_sim.h"

FEniCS_Blood_Sim::FEniCS_Blood_Sim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FEniCS_Blood_Sim)
{
    ui->setupUi(this);
    // Data
    _projectData = new FBS_ProjectData();
    // FileMenu
    _fileMenuBuilder = new FileMenuBuilder();
    _fileMenuBuilder->registerWindow(this);
    _fileMenuBuilder->registerProjectData(_projectData);

}

FEniCS_Blood_Sim::~FEniCS_Blood_Sim()
{
    delete ui;
    delete _fileMenuBuilder;
}



// -------------------------------------------------
// FILE MENU


void FEniCS_Blood_Sim::on_actionNew_Project_triggered()
{
    _fileMenuBuilder->launchMenuAction(NEW_PROJECT);
    updateStatusBar();

}

void FEniCS_Blood_Sim::on_actionOpen_Project_triggered()
{
    _fileMenuBuilder->launchMenuAction(OPEN_PROJECT);
    updateStatusBar();

}

void FEniCS_Blood_Sim::on_actionRecent_Projects_triggered()
{
    _fileMenuBuilder->launchMenuAction(RECENT_PROJECTS);

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
    _fileMenuBuilder->launchMenuAction(EXIT_APP);

}

void FEniCS_Blood_Sim::on_actionExit_MainWindow_triggered()
{
    _fileMenuBuilder->launchMenuAction(EXIT_MAINWINDOW);

}


//**************************************************************
void FEniCS_Blood_Sim::updateStatusBar()
{
    QString _message = "Project: " + _projectData->getProjectName();
    statusBar()->showMessage(_message);
}
