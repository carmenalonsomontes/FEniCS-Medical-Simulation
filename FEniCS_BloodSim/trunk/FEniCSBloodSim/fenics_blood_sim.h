#ifndef FENICS_BLOOD_SIM_H
#define FENICS_BLOOD_SIM_H

#include <QMainWindow>

#include "GUI_Module/Menu/filemenubuilder.h"
#include "GUI_Module/Defines/Menu/MenuDefines.h"
#include "GUI_Module/Data/FBS_Project/fbs_projectdata.h"
#include "GUI_Module/Session/usersessiondata.h"
#include "GUI_Module/Menu/Simulation/imageprocessingsimmenu.h"

// Image Processing Libs
#include "ImageProcessing_Module/imagedata.h"

// VTK libs
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <QVTKWidget.h>

#include <vtkImageViewer2.h>

#include "GUI_Module/UIHelpers/workflowtablehelper.h"

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
   // bool eventFilter(QObject *obj, QEvent *event);

private slots:
    // EDIT
    void on_actionNew_Project_triggered();

    void on_actionOpen_Project_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionClose_Project_triggered();

    void on_actionExit_triggered();

    void on_actionExit_MainWindow_triggered();

    void exitApplicationUI();
    void UpdateStatusBar(const QString text);

    void EnableCloseProjectUI(bool val);
    void EnableSaveProjectUI(bool val);
    void RestoreUI();
    void EnableMedicalImagingFrame(bool val);

    void EnableTab(bool val);

    void UpdateConsole(const QString text);

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

    void UpdateImageConsole(const QString text);


    void EnableImageProcessingDialog(bool val);

    void LoadImageInterfaceUI(const QString imPath);



    // ---------------------------------------------------------
    // MESH TAB
    void on_meshButton_clicked();

    void on_meshDoneButton_clicked();

    void on_actionAbout_FEniCs_Blood_Sim_triggered();



    void on_axialSlider_sliderMoved(int position);

    void on_coronalSlider_sliderMoved(int position);

    void on_sagittalSlider_sliderMoved(int position);

    void on_workflowTableWidget_cellClicked(int row, int column);



    void on_workflowConfigButton_clicked();

private:
    Ui::FEniCS_Blood_Sim *ui;
    FileMenuBuilder *  _fileMenuBuilder;
    ImageProcessingSimMenu * _imProcMenuBuilder;
    WorkflowTableHelper _workflowTableHelper;

    // Data
    FBS_ProjectData * _projectData;
    UserSessionData * _userSessionData;



    vtkSmartPointer<vtkRenderer> mainImRendererTab;

    vtkSmartPointer<vtkImageViewer2> axialImViewer;
    vtkSmartPointer<vtkImageViewer2> coronalImViewer;
    vtkSmartPointer<vtkImageViewer2> sagittalImViewer;



    vtkSmartPointer<vtkRenderer> cloudImRenderer;
    vtkSmartPointer<vtkRenderer> meshImRenderer;

    vtkSmartPointer<vtkRenderer> visualizationRenderer;





    // Render images


   // void installEventFilters();
    void connectSignalsMenuBuilder();
    void loadUserSessionData();
    void closeApplication();

    void fillRecentProjectsSection();
    QAction *  createRecentProjectAction(QString _name);
    void   openImageProcessingWindow();
    void ClearImageInterfaceUI();


    // Image Tab - Load
    void LoadAxialImage();
    void LoadSaggitalImage();
    void LoadCoronalImage();
    void LoadMainImageTab();

    // Image Tab - Viewer
    void loadViewer(QVTKWidget * widget, vtkSmartPointer<vtkImageViewer2> imageViewer);
    void clearViewer(QVTKWidget * widget, vtkSmartPointer<vtkImageViewer2> imageViewer);
    void setOrientation(vtkSmartPointer<vtkImageViewer2> imageViewer,int orientation);

    // Image Tab - Slicer
    void updateSlicerMinMax(int noSlice, int max, int min, int typeSlicer);
    void resetSliders();

    // Clear functions - Tab
    void clearImageTab();
    void clearMeshTab();
    void clearFenicsTab();
    void clearSimulationTab();
    void clearVisualizationTab();
    void ClearConsoles();

    // Initialize functions-  Tab
    void initializeImageTab();
    void initializeMeshTab();
    void initializeFenicsTab();
    void initializeSimulationTab();
    void initializeVisualizationTab();




};

#endif // FENICS_BLOOD_SIM_H



