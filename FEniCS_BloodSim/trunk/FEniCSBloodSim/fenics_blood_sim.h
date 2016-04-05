#ifndef FENICS_BLOOD_SIM_H
#define FENICS_BLOOD_SIM_H

#include <QMainWindow>

#include "GUI_Module/Menu/filemenubuilder.h"
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "GUI_Module/Data/FBS_Project/fbs_projectdata.h"
#include "GUI_Module/Session/usersessiondata.h"
#include "GUI_Module/Menu/Simulation/imageprocessingsimmenu.h"

// VTK libs
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

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

protected:
    bool eventFilter(QObject *obj, QEvent *event);

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
    void RestoreUI();
    void EnableMedicalImagingFrame(bool val);

    void EnableTab(bool val);

    // RECENT PROJECTS
    void on_actionClear_list_triggered();

    void updateRecentProjectListUI(const QString projectPath);
    void openRecentFile();

    // PREFERENCES
    void on_actionPreferences_triggered();

    // MEDICAL IMAGING INTERFACE - MAIN WINDOW

    void on_setDataPathButton_clicked();

    void on_setImButton_clicked();

    void on_setPrefixSeriesButton_clicked();

    void on_setMeshToolPathButton_clicked();

    void on_setFenicsToolPathButton_clicked();

    void on_setVisualizationToolPathButton_clicked();

    void on_actionImage_Dataset_triggered();

    void UpdateImagingDialog(const QString text);

    void EnableImageProcessingDialog(bool val);

    void LoadImageInterfaceUI(const QString imPath);
private:
    Ui::FEniCS_Blood_Sim *ui;
    FileMenuBuilder *  _fileMenuBuilder;
    ImageProcessingSimMenu * _imProcMenuBuilder;

    // Data
    FBS_ProjectData * _projectData;
    UserSessionData * _userSessionData;

    vtkSmartPointer<vtkRenderer> mainImRendererTab;
    vtkSmartPointer<vtkRenderer> coronalImRendererTab;
    vtkSmartPointer<vtkRenderer> axialImRendererTab;
    vtkSmartPointer<vtkRenderer> saggitalImRendererTab;

    // Render images


    void installEventFilters();
    void connectSignalsMenuBuilder();
    void loadRecentProjectList();
    void closeApplication();

    void fillRecentProjectsSection();
    QAction *  createRecentProjectAction(QString _name);
    void   openImageProcessingWindow();
    void ClearImageInterfaceUI();



    void LoadAxialImage();
    void LoadSaggitalImage();
    void LoadCoronalImage();

};

#endif // FENICS_BLOOD_SIM_H



