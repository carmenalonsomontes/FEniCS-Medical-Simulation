#ifndef FENICS_BLOOD_SIM_H
#define FENICS_BLOOD_SIM_H

#include <QMainWindow>

#include "GUI_Module/Menu/filemenubuilder.h"
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "GUI_Module/Data/FBS_Project/fbs_projectdata.h"

namespace Ui {
class FEniCS_Blood_Sim;
}

class FEniCS_Blood_Sim : public QMainWindow
{
    Q_OBJECT

public:
    explicit FEniCS_Blood_Sim(QWidget *parent = 0);
    ~FEniCS_Blood_Sim();

private slots:
    // EDIT
    void on_actionNew_Project_triggered();

    void on_actionOpen_Project_triggered();

    void on_actionRecent_Projects_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionClose_Project_triggered();

    void on_actionExit_triggered();

    void on_actionExit_MainWindow_triggered();

private:
    Ui::FEniCS_Blood_Sim *ui;
    FileMenuBuilder *  _fileMenuBuilder;


    // Data
    FBS_ProjectData * _projectData;


    void updateStatusBar();
};

#endif // FENICS_BLOOD_SIM_H



