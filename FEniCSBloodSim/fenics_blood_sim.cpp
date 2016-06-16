#include "fenics_blood_sim.h"
#include "ui_fenics_blood_sim.h"

#include <QDir>

#include <QAction>
#include <vtkPolyDataMapper.h>

#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>

#include "ImageProcessing_Module/imagereader.h"

// FORMs
#include "GUI_Module/Forms/preferencesdialog.h"
#include "GUI_Module/Forms/aboutdialog.h"

// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkNiftiImageIO.h>

// VTK
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkImageMapper3D.h"

#include "vtkImageSliceMapper.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolumeRayCastCompositeFunction.h"

#include "vtkSmartVolumeMapper.h"

#include "vtkImageReader2Factory.h"
#include "vtkImageReader2.h"
#include "vtkNIFTIImageReader.h"

// My libraries
#include "GUI_Module/UIHelpers/workflowtablehelper.h"
#include "GUI_Module/Forms/Workflow/imageworkflow.h"


using namespace std;
using namespace itk;

FEniCS_Blood_Sim::FEniCS_Blood_Sim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FEniCS_Blood_Sim)
{
    ui->setupUi(this);
    // Data
    _projectData = new FBS_ProjectData();
    _userSessionData = new UserSessionData();
    _imProcMenuBuilder = new ImageProcessingSimMenu();


    // FileMenu
    _fileMenuBuilder = new FileMenuBuilder();
    _fileMenuBuilder->registerWindow(this);
    _fileMenuBuilder->registerProjectData(_projectData);

    // Image Processing Menu
    _imProcMenuBuilder->registerWindow(this);
    _imProcMenuBuilder->registerProjectData(_projectData);

    // Helpers
    _workflowTableHelper.registerTableUI(ui->workflowTableWidget);

   // _currentRowVisibleItem = -1;

    // ----------------------------------------------------
    // VTK initializations
    //

    initializeImageTab();
    initializeMeshTab();
    initializeFenicsTab();
    initializeSimulationTab();
    initializeVisualizationTab();
    connectSignalsMenuBuilder();
    loadUserSessionData();
    //installEventFilters();
  }



FEniCS_Blood_Sim::~FEniCS_Blood_Sim()
{
    delete ui;
    // Builders
    delete _fileMenuBuilder;
    delete _imProcMenuBuilder;


    // Data
    delete _projectData;
    delete _userSessionData;



}

void FEniCS_Blood_Sim::connectSignalsMenuBuilder()
{
     connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(exitApplicationUI()));
    // File Menu Builder signals & Slots
     connect(_fileMenuBuilder, SIGNAL(updateStatusBarUI(const QString)), this, SLOT(UpdateStatusBar(const QString)));
     connect(_fileMenuBuilder, SIGNAL(enableCloseProjectUI(bool)), this, SLOT(EnableCloseProjectUI(bool)));
     connect(_fileMenuBuilder, SIGNAL(enableSaveProjectUI(bool)), this, SLOT(EnableSaveProjectUI(bool)));
     //connect(_fileMenuBuilder, SIGNAL(exittUI()), this, SLOT(exitApplicationUI()));
     connect(_fileMenuBuilder, SIGNAL(updateRecentProjectList(const QString)), this, SLOT(updateRecentProjectListUI(const QString)));
     connect(_fileMenuBuilder, SIGNAL(restoreUI()), this, SLOT(RestoreUI()));
     connect(_fileMenuBuilder, SIGNAL(updateImagingDialogUI(const QString)), this, SLOT(UpdateImagingDialog(const QString)));
     connect(_fileMenuBuilder, SIGNAL(enableMedicalImagingFrameUI(bool)), this, SLOT(EnableMedicalImagingFrame(bool)));
     connect(_fileMenuBuilder, SIGNAL(enableImageProcessingDialogUI(bool)), this, SLOT(EnableImageProcessingDialog(bool)));
     connect(_fileMenuBuilder, SIGNAL(loadImageInterface(const QString)), this, SLOT(LoadImageInterfaceUI(const QString)));
     connect(_fileMenuBuilder, SIGNAL(enableTabUI(bool)), this, SLOT(EnableTab(bool)));
     connect(_fileMenuBuilder, SIGNAL(updateConsoleUI(const QString)), this, SLOT(UpdateConsole(const QString)));


     // Medical Imaging Signals
     connect(_imProcMenuBuilder, SIGNAL(updateImagingDialogUI(const QString)), this, SLOT(UpdateImagingDialog(const QString)));
     connect(_imProcMenuBuilder, SIGNAL(updateImagingConsole(const QString)), this, SLOT(UpdateImageConsole(const QString)));
     connect(_imProcMenuBuilder, SIGNAL(enableSaveProjectUI(bool)), this, SLOT(EnableSaveProjectUI(bool)));
     connect(_imProcMenuBuilder, SIGNAL(loadImageInterface(const QString)), this, SLOT(LoadImageInterfaceUI(const QString)));

}




void FEniCS_Blood_Sim::loadUserSessionData()
{
    QString _path = QDir(QDir::currentPath()).filePath(PREFERENCES_FILE_NAME);
    _userSessionData->setFileNamePath(_path);
    _userSessionData->loadAllData();
    fillRecentProjectsSection();

}

void FEniCS_Blood_Sim::initializeImageTab()
{
    mainImRendererTab =  vtkSmartPointer<vtkRenderer>::New();
    coronalImViewer =  vtkSmartPointer<vtkImageViewer2>::New();
    axialImViewer =  vtkSmartPointer<vtkImageViewer2>::New();
    sagittalImViewer =  vtkSmartPointer<vtkImageViewer2>::New();

}

void FEniCS_Blood_Sim::initializeMeshTab()
{
    cloudImRenderer = vtkSmartPointer<vtkRenderer>::New();
    meshImRenderer = vtkSmartPointer<vtkRenderer>::New();
}


void FEniCS_Blood_Sim::initializeFenicsTab()
{
    // TODO
}

void FEniCS_Blood_Sim::initializeSimulationTab()
{
    // TODO
}

void FEniCS_Blood_Sim::initializeVisualizationTab()
{
    visualizationRenderer = vtkSmartPointer<vtkRenderer>::New();
    ui->visualizationWidget->setEnabled(false);
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

void  FEniCS_Blood_Sim::exitApplicationUI()
{
    closeApplication();
}

//**************************************************************
/* RECENT PROJECTS */
void FEniCS_Blood_Sim::on_actionClear_list_triggered()
{

    if (_userSessionData->isEmptyRecentProjects()) return;
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
// EDIT MENU
void FEniCS_Blood_Sim::on_actionPreferences_triggered()
{
    PreferencesDialog _preferencesDialog;
    _preferencesDialog.setMeshPath(_userSessionData->getMeshToolPath());
    _preferencesDialog.setFENICSPath(_userSessionData->getFenicsToolPath());
    _preferencesDialog.exec();

    // Update
    if(_preferencesDialog.userAcceptChanges())
    {
        _userSessionData->setMeshToolPath(_preferencesDialog.getMeshPath());
        _userSessionData->setFenicsToolPath(_preferencesDialog.getFENICSPath());
    }
}



//********************************************************************
// PROJECT SETUP

// MENU
void FEniCS_Blood_Sim::on_actionImage_Dataset_triggered()
{
    _imProcMenuBuilder->launchMenuAction(IMAGE_PROCESSING);
}



//--
// MEDICAL IMAGES
void FEniCS_Blood_Sim::on_setDataPathButton_clicked()
{
    openImageProcessingWindow();
}

void FEniCS_Blood_Sim::on_setImButton_clicked()
{
    openImageProcessingWindow();

}

void FEniCS_Blood_Sim::on_setPrefixSeriesButton_clicked()
{
    openImageProcessingWindow();

}

void FEniCS_Blood_Sim::openImageProcessingWindow()
{
    if (_projectData->isEmptyImagingData())
        _imProcMenuBuilder->launchMenuAction(IMAGE_PROCESSING);
    else
        // At the moment, we only allow for a single image. Otherwise, we need to fulfill
        // the other fields: ImPrefix
        _imProcMenuBuilder->launchMenuAction(IMAGE_PROCESSING,_projectData->getImPath());

    _projectData->getImageData()->loadImageData(_projectData->getImPath());


}

void FEniCS_Blood_Sim::UpdateImagingDialog(const QString text)
{
    if ((text.isEmpty()) || (text.isNull()))
        return;
    QString _fileName = QFileInfo(text).fileName();
    QString _filePath = QFileInfo(text).absolutePath();

    //Save to the PROJECT DATA
    _projectData->setImPath(text);

    // Update UI
    ui->imNamelineEdit->setText(_fileName);
    ui->datasetPathlineEdit->setText(_filePath);    

}

void FEniCS_Blood_Sim::UpdateImageConsole(const QString text)
{
    ui->mainTabWidget->setCurrentIndex(IMAGE_TAB_INDEX);
    ui->imageConsoleTextEdit->append(text);

}


// -------------------------------------------------------------
// IMAGING LOGIC
// -------------------------------------------------------------
//--
// MESH TOOL
void FEniCS_Blood_Sim::on_setMeshToolPathButton_clicked()
{
    _imProcMenuBuilder->launchMenuAction(MESH_TOOL);
}


//--
// FENICS
void FEniCS_Blood_Sim::on_setFenicsToolPathButton_clicked()
{
    _imProcMenuBuilder->launchMenuAction(FENICS_TOOL);
}

//--
// VISUALIZATION TOOL

void FEniCS_Blood_Sim::on_setVisualizationToolPathButton_clicked()
{
    _imProcMenuBuilder->launchMenuAction(VISUALIZATION_TOOL);
}




//**************************************************************
void FEniCS_Blood_Sim::closeApplication()
{
    _projectData->saveProjectInfoToFile();
    _userSessionData->saveAllData();
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
void FEniCS_Blood_Sim::UpdateConsole(const QString text)
{
   int _tabIndex =  ui->mainTabWidget->currentIndex();

   switch (_tabIndex)
   {
       case IMAGE_TAB_INDEX:
        ui->imageConsoleTextEdit->append(text);
           break;
       case MESH_TAB_INDEX:
       ui->meshConsole->append(text);
           break;
       case FENICS_TAB_INDEX:
       ui->fenicsConsole->append(text);
           break;
       case SIMULATION_TAB_INDEX:
       ui->simulationConsole->append(text);
           break;
   };
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
void FEniCS_Blood_Sim::EnableMedicalImagingFrame(bool val)
{
    ui->mediImSegFrame->setEnabled(val);
    ui->imNamelineEdit->setEnabled(val);
    ui->datasetPathlineEdit->setEnabled(val);

}
void FEniCS_Blood_Sim::EnableImageProcessingDialog(bool val)
{
    ui->imageProcessingFrame->setEnabled(val);
    if (!_projectData->isEmptyImagingData())
    {
        QString msg = "Original File: " + QFileInfo(_projectData->getImPath()).baseName();
        _workflowTableHelper.addElementToTable(msg,EYE_OPEN);

        if (!_projectData->isEmptyPipelineData())
            addPipelineItems();

    }else
    {
        _workflowTableHelper.clearTable();
        _workflowTableHelper.clearEyeAssociateddTable();
    }
}

void FEniCS_Blood_Sim::EnableTab(bool val)
{
    ui->mainTabWidget->setEnabled(val);
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

void FEniCS_Blood_Sim::RestoreUI()
{
    //Clear main UI line edits
    ui->imNamelineEdit->clear();
    ui->datasetPathlineEdit->clear();

    // Set active tab in imaging
    ui->mainTabWidget->setCurrentIndex(IMAGE_TAB_INDEX);

    EnableMedicalImagingFrame(false);
    EnableImageProcessingDialog(false);
    EnableTab(false);

    ClearImageInterfaceUI();
    ClearConsoles();

   // _currentRowVisibleItem = -1;
}

void FEniCS_Blood_Sim::ClearConsoles()
{
    ui->imageConsoleTextEdit->clear();
    ui->meshConsole->clear();
    ui->fenicsConsole->clear();
    ui->simulationConsole->clear();
}





/**********************************************************
 * IMAGING
 * */


 void FEniCS_Blood_Sim::ClearImageInterfaceUI()
 {
    clearImageTab();
    clearMeshTab();
    clearFenicsTab();
    clearFenicsTab();
    clearSimulationTab();
    clearVisualizationTab();
 }



 void FEniCS_Blood_Sim::clearImageTab()
 {

     // Set Black Images
     if (this->ui->mainImageWidget->GetRenderWindow()->HasRenderer(mainImRendererTab))
     {
         mainImRendererTab->SetBackground(0,0,0);
         mainImRendererTab->RemoveAllViewProps();
         mainImRendererTab->ResetCamera();
     }
    ui->mainImageWidget->setEnabled(false);

    clearViewer(ui->axialViewWidget,axialImViewer);
    clearViewer(ui->sagittalViewWidget,sagittalImViewer);
    clearViewer(ui->coronalViewWidget,coronalImViewer);

    resetSliders();
 }

 void FEniCS_Blood_Sim::resetSliders()
 {
     ui->axialSlider->setValue(0);
     ui->coronalSlider->setValue(0);
     ui->sagittalSlider->setValue(0);

     // Reseting the values
     ui->currentAxialSlice->setText(QString::number(0));
     ui->totalAxialSlices->setText("...");
     ui->currentCoronalSlice->setText(QString::number(0));
     ui->totalCoronalSlices->setText("...");
     ui->currentSagittalSlice->setText(QString::number(0));
     ui->totalSagittalSlices->setText("...");
 }

 void FEniCS_Blood_Sim::clearViewer(QVTKWidget * widget, vtkSmartPointer<vtkImageViewer2> imageViewer)
 {
     imageViewer->GetRenderer()->RemoveViewProp(imageViewer->GetImageActor());
     imageViewer->GetRenderer()->ResetCamera();
     widget->setEnabled(false);
 }



 void FEniCS_Blood_Sim::clearMeshTab()
 {

     ui->cloudPointWidget->setEnabled(false);
     ui->meshWidget->setEnabled(false);
     // Setting up the renderer
     cloudImRenderer->SetBackground(0,0,0);
     cloudImRenderer->RemoveAllViewProps();
     cloudImRenderer->ResetCamera();
     ui->cloudPointWidget->GetRenderWindow()->AddRenderer(cloudImRenderer);

     meshImRenderer->SetBackground(0,0,0);
     meshImRenderer->RemoveAllViewProps();
     meshImRenderer->ResetCamera();
     ui->meshWidget->GetRenderWindow()->AddRenderer(meshImRenderer);

 }

 void FEniCS_Blood_Sim::clearFenicsTab()
 {

 }

 void FEniCS_Blood_Sim::clearSimulationTab()
 {

 }

 void FEniCS_Blood_Sim::clearVisualizationTab()
 {
    ui->visualizationWidget->setEnabled(false);

    visualizationRenderer->SetBackground(0,0,0);
    visualizationRenderer->RemoveAllViewProps();
    visualizationRenderer->ResetCamera();
    ui->visualizationWidget->GetRenderWindow()->AddRenderer(visualizationRenderer);

 }



 void FEniCS_Blood_Sim::LoadMainImageTab(vtkVolume * _volume)
 {
     if (!ui->mainImageWidget->isEnabled())
         ui->mainImageWidget->setEnabled(true);

    mainImRendererTab->SetBackground(128,128,128);
    mainImRendererTab->RemoveAllViewProps();
    mainImRendererTab->ResetCamera();
    // Adding the volume
    mainImRendererTab->AddVolume(_volume);
    mainImRendererTab->ResetCamera();
    ui->mainImageWidget->GetRenderWindow()->AddRenderer(mainImRendererTab);
 }


 void FEniCS_Blood_Sim::LoadAxialImage(vtkImageData * _imageData)
 {
    loadViewer( ui->axialViewWidget, axialImViewer,_imageData);
    updateSlicerSesionInfo(axialImViewer,AXIAL_TAB);

 }


 void FEniCS_Blood_Sim::LoadSaggitalImage(vtkImageData * _imageData)
 {
    loadViewer( ui->sagittalViewWidget, sagittalImViewer,_imageData);
    setOrientation(sagittalImViewer,SAGITTAL_YZ);
    updateSlicerSesionInfo(sagittalImViewer,SAGITTAL_TAB);

 }

 void FEniCS_Blood_Sim::LoadCoronalImage(vtkImageData * _imageData)
 {
    loadViewer( ui->coronalViewWidget, coronalImViewer,_imageData);
    setOrientation(coronalImViewer,CORONAL_XZ);
    updateSlicerSesionInfo(coronalImViewer,CORONAL_TAB);
 }


 void FEniCS_Blood_Sim::updateSlicerSesionInfo(vtkSmartPointer<vtkImageViewer2> imageViewer,ImageTabs typeSlice)
 {
     int _currentRowVisibleItem = _workflowTableHelper.getCurrentRowWithOpenEye();


     Slicer_no _sliceTabNo;
     switch (typeSlice) {
     case AXIAL_TAB:
         _sliceTabNo = AXIAL_SLICER;
         break;
     case SAGITTAL_TAB:
         _sliceTabNo = SAGITTAL_SLICER;
         break;
     case CORONAL_TAB:
         _sliceTabNo = CORONAL_SLICER;
         break;
     default:
         break;
     }

     if ( (_currentRowVisibleItem != -1) && (! _userSessionData->existImageInfo(_currentRowVisibleItem,typeSlice)))
        _userSessionData->setImageTabsInfo(_currentRowVisibleItem,typeSlice,imageViewer->GetSlice(),imageViewer->GetSliceMax(),imageViewer->GetSliceMin());


     UserImageInfoSession _imageInfo = _userSessionData->getImageTabsInfo(_currentRowVisibleItem,typeSlice);
     setSliceNumber(imageViewer,_imageInfo.getNoSlice());
     updateSlicerMinMax(_imageInfo.getNoSlice(),_imageInfo.getMaxSlice(),_imageInfo.getMinSlice(),_sliceTabNo);
 }

 void FEniCS_Blood_Sim::updateSlicerMinMax(int noSlice, int max, int min, int typeSlicer)
 {
     blockSignals(true);

     switch (typeSlicer) {
     case AXIAL_SLICER:
         ui->axialSlider->setMaximum(max);
         ui->axialSlider->setMinimum(min);
         ui->totalAxialSlices->setText(QString::number(max));
         ui->currentAxialSlice->setText(QString::number(noSlice));
         ui->axialSlider->setValue(noSlice);
         break;
     case SAGITTAL_SLICER:
         ui->sagittalSlider->setMaximum(max);
         ui->sagittalSlider->setMinimum(min);
         ui->totalSagittalSlices->setText(QString::number(max));
         ui->currentSagittalSlice->setText(QString::number(noSlice));
         ui->sagittalSlider->setValue(noSlice);
         break;
     case CORONAL_SLICER:
         ui->coronalSlider->setMaximum(max);
         ui->coronalSlider->setMinimum(min);
         ui->totalCoronalSlices->setText(QString::number(max));
         ui->currentCoronalSlice->setText(QString::number(noSlice));
         ui->coronalSlider->setValue(noSlice);
         break;
     }

    blockSignals(false);
 }


 void FEniCS_Blood_Sim::setOrientation(vtkSmartPointer<vtkImageViewer2> imageViewer,int orientation)
 {
     switch (orientation) {
     case SAGITTAL_YZ:
         imageViewer->SetSliceOrientationToYZ();
         break;
     case CORONAL_XZ:
         imageViewer->SetSliceOrientationToXZ();
         break;
     }

     imageViewer->GetRenderer()->ResetCamera();
     imageViewer->Render();
 }

 void FEniCS_Blood_Sim::setSliceNumber(vtkSmartPointer<vtkImageViewer2> imageViewer,int sliceNo)
 {
     imageViewer->SetSlice(sliceNo);
     imageViewer->GetRenderer()->ResetCamera();
     imageViewer->Render();
 }


 void FEniCS_Blood_Sim::LoadImageInterfaceUI(const QString imPath)
 {
    if (imPath.isEmpty())
        return;
     // Loading image
    _projectData->setImPath(imPath);
    _projectData->loadImData();

    // Updating the UI
    loadImages(_projectData->getImageData()->getVolumeData(), _projectData->getImageData()->getImageData());
   }


void FEniCS_Blood_Sim::loadViewer(QVTKWidget * widget, vtkSmartPointer<vtkImageViewer2> imageViewer, vtkImageData * _imageData)
{
    if (!widget->isEnabled())
        widget->setEnabled(true);

     imageViewer->SetInputData(_imageData);

     if (!imageViewer->GetRenderer()->HasViewProp(imageViewer->GetImageActor()) )
         imageViewer->GetRenderer()->AddViewProp(imageViewer->GetImageActor());

     imageViewer->SetupInteractor(widget->GetRenderWindow()->GetInteractor());
     imageViewer->SetRenderWindow(widget->GetRenderWindow());

     imageViewer->GetRenderer()->ResetCamera();
     imageViewer->Render();
     widget->update();

}



// ------------------------------------------------------------------------------------------------------
//          MESH TAB

void FEniCS_Blood_Sim::on_meshButton_clicked()
{

}

void FEniCS_Blood_Sim::on_meshDoneButton_clicked()
{

}

void FEniCS_Blood_Sim::on_actionAbout_FEniCs_Blood_Sim_triggered()
{
    AboutDialog _aboutDialog;
    _aboutDialog.exec();
}


// ------------------------------------------------------------------------------------------------------
// AXIAL
void FEniCS_Blood_Sim::on_axialSlider_sliderMoved(int position)
{
   axialImViewer->SetSlice(position);
   ui->currentAxialSlice->setText(QString::number(position));


}

void FEniCS_Blood_Sim::on_coronalSlider_sliderMoved(int position)
{
   coronalImViewer->SetSlice(position);
   ui->currentCoronalSlice->setText(QString::number(position));

}

void FEniCS_Blood_Sim::on_sagittalSlider_sliderMoved(int position)
{
    sagittalImViewer->SetSlice(position);
    ui->currentSagittalSlice->setText(QString::number(position));

}

void FEniCS_Blood_Sim::on_axialSlider_sliderReleased()
{
   saveImageSessionData(AXIAL_TAB,axialImViewer->GetSlice(),axialImViewer->GetSliceMax(),axialImViewer->GetSliceMin());
}

void FEniCS_Blood_Sim::on_coronalSlider_sliderReleased()
{
    saveImageSessionData(CORONAL_TAB,coronalImViewer->GetSlice(),coronalImViewer->GetSliceMax(),coronalImViewer->GetSliceMin());
}

void FEniCS_Blood_Sim::on_sagittalSlider_sliderReleased()
{
     saveImageSessionData(SAGITTAL_TAB,sagittalImViewer->GetSlice(),sagittalImViewer->GetSliceMax(),sagittalImViewer->GetSliceMin());
}

void FEniCS_Blood_Sim::saveImageSessionData(ImageTabs typeView, int noSlice,int maxSlice, int minSlice)
{
    int _currentRowSelected = _workflowTableHelper.getCurrentRowWithOpenEye();
    _userSessionData->setImageTabsInfo(_currentRowSelected,typeView,noSlice,maxSlice,minSlice);

}


// -----------------------------------------------------------------------------------------------------------
//  workflow
void FEniCS_Blood_Sim::on_workflowTableWidget_cellClicked(int row, int column)
{
    if (column == EYE_COLUMN)  
        showImage(row);

}

#include <itkImageToVTKImageFilter.h>
void FEniCS_Blood_Sim::showImage(int row)
{

    int _currentRowVisibleItem = _workflowTableHelper.getCurrentRowWithOpenEye();

    if (_currentRowVisibleItem == -1) return;

    if (row != _currentRowVisibleItem)
    {
         //clearImageTab();
         _workflowTableHelper.modifyEyeInRow(row);
         _workflowTableHelper.modifyEyeInRow(_currentRowVisibleItem);
         clearImageTab();
         if (row == 0) // Volume Data
            loadImages(_projectData->getImageData()->getVolumeData(), _projectData->getImageData()->getImageData());
          else
             if( (!_projectData->getListPipelineItems().isEmpty()) && ((row -1) <= _projectData->getListPipelineItems().size()))
             {
                 QList<PipelineItem> _imagingPipelineItems = _projectData->getListPipelineItems();

                 PipelineItem _item = _imagingPipelineItems.at(row-1);

                 if (!_item.isImageLoaded())
                    _item.loadImage();
                 ImageData * _itemImData = _item.getImageData();

                 // Show the images
                 loadImages(_itemImData->getVolumeData(), _itemImData->getImageData());
             }
    }
}


void FEniCS_Blood_Sim::loadImages(vtkVolume * _volume, vtkImageData * _imageData)
{
   // clearImageTab();
    LoadMainImageTab(_volume);
    LoadAxialImage(_imageData);
    LoadSaggitalImage(_imageData);
    LoadCoronalImage(_imageData);
}

void FEniCS_Blood_Sim::on_workflowConfigButton_clicked()
{
    // TODO
    ImageWorkflow _imgWkf;
    _imgWkf.setImagePath(_projectData->getImPath());
    _imgWkf.setUserProjectPath(_projectData->getProjectPath());
    _imgWkf.setUserImageData(_projectData->getImageData());
    if (!_projectData->getListPipelineItems().isEmpty())
        _imgWkf.setPipelineItemList(_projectData->getListPipelineItems());
    _imgWkf.exec();

    if (_imgWkf.userAcceptChanges())
    {
        _workflowTableHelper.removeRowsFromThisOnwards(1);
        _projectData->setListPipelineItems(_imgWkf.getPipelineItemList());
        addPipelineItems();
        EnableSaveProjectUI(true);
    }

}


void FEniCS_Blood_Sim::addPipelineItems()
{

    for (int i = 0;i <_projectData->getListPipelineItems().size();i++)
    {
        PipelineItem  _item = _projectData->getListPipelineItems().at(i);
        _workflowTableHelper.addElementToTable(_item.getDescription(),EYE_CLOSED);
    }
}


/*
void FEniCS_Blood_Sim::on_axialSlider_valueChanged(int value)
{
    // Saving the data for the session
    saveImageSessionData(AXIAL_TAB,axialImViewer->GetSlice(),axialImViewer->GetSliceMax(),axialImViewer->GetSliceMin());
}

*/


// ------------------------------------------------------------------------------------------
// FENICS
#include "GUI_Module/Forms/FEniCS/fenicsconfigurationdialog.h"

void FEniCS_Blood_Sim::on_fenicsConfigureButton_clicked()
{
    FEniCSConfigurationDialog _fenicsConfiguration;
    _fenicsConfiguration.exec();

}
