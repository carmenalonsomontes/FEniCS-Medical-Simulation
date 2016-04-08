#include "fenics_blood_sim.h"
#include "ui_fenics_blood_sim.h"

#include <QDir>
#include <QAction>
#include <vtkPolyDataMapper.h>

#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>

#include "ImageProcessing_Module/imagereader.h"

// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkNiftiImageIO.h>

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
    _imageData = new ImageData();

    // FileMenu
    _fileMenuBuilder = new FileMenuBuilder();
    _fileMenuBuilder->registerWindow(this);
    _fileMenuBuilder->registerProjectData(_projectData);

    // Image Processing Menu
    _imProcMenuBuilder->registerWindow(this);
    _imProcMenuBuilder->registerProjectData(_projectData);

    // VTK initializations
    mainImRendererTab =  vtkSmartPointer<vtkRenderer>::New();
    coronalImRendererTab =  vtkSmartPointer<vtkRenderer>::New();
    axialImRendererTab =  vtkSmartPointer<vtkRenderer>::New();
    saggitalImRendererTab =  vtkSmartPointer<vtkRenderer>::New();

    connectSignalsMenuBuilder();
    loadRecentProjectList();
    installEventFilters();

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
    delete _imageData;

}

void FEniCS_Blood_Sim::installEventFilters()
{
    ui->axialViewWidget->installEventFilter(this);
    ui->sagittalViewWidget->installEventFilter(this);
    ui->coronalViewWidget->installEventFilter(this);
}

void FEniCS_Blood_Sim::connectSignalsMenuBuilder()
{
     connect(_fileMenuBuilder, SIGNAL(updateStatusBarUI(const QString)), this, SLOT(UpdateStatusBar(const QString)));
     connect(_fileMenuBuilder, SIGNAL(enableCloseProjectUI(bool)), this, SLOT(EnableCloseProjectUI(bool)));
     connect(_fileMenuBuilder, SIGNAL(enableSaveProjectUI(bool)), this, SLOT(EnableSaveProjectUI(bool)));
     connect(_fileMenuBuilder, SIGNAL(exittUI()), this, SLOT(exitApplicationUI()));
     connect(_fileMenuBuilder, SIGNAL(updateRecentProjectList(const QString)), this, SLOT(updateRecentProjectListUI(const QString)));
     connect(_fileMenuBuilder, SIGNAL(restoreUI()), this, SLOT(RestoreUI()));
     connect(_fileMenuBuilder, SIGNAL(updateImagingDialogUI(const QString)), this, SLOT(UpdateImagingDialog(const QString)));
     connect(_fileMenuBuilder, SIGNAL(enableMedicalImagingFrameUI(bool)), this, SLOT(EnableMedicalImagingFrame(bool)));
     connect(_fileMenuBuilder, SIGNAL(enableImageProcessingDialogUI(bool)), this, SLOT(EnableImageProcessingDialog(bool)));
     connect(_fileMenuBuilder, SIGNAL(loadImageInterface(const QString)), this, SLOT(LoadImageInterfaceUI(const QString)));
     connect(_fileMenuBuilder, SIGNAL(enableTabUI(bool)), this, SLOT(EnableTab(bool)));


     // Medical Imaging Signals
     connect(_imProcMenuBuilder, SIGNAL(updateImagingDialogUI(const QString)), this, SLOT(UpdateImagingDialog(const QString)));


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
// EDIT MENU
void FEniCS_Blood_Sim::on_actionPreferences_triggered()
{
    // TODO
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

    // At the moment, we only allow for a single image. Otherwise, we need to fulfill
    // the other fields: ImPrefix
    _imProcMenuBuilder->launchMenuAction(IMAGE_PROCESSING,_projectData->getImPath());

    ImageReader _imReader;
    _imReader.readImage(_imageData,_projectData->getImPath());
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
void FEniCS_Blood_Sim::EnableMedicalImagingFrame(bool val)
{
    ui->MediImSegFrame->setEnabled(val);
    ui->imNamelineEdit->setEnabled(val);
    ui->datasetPathlineEdit->setEnabled(val);

}
void FEniCS_Blood_Sim::EnableImageProcessingDialog(bool val)
{
    ui->imageProcessingFrame->setEnabled(val);
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

    EnableMedicalImagingFrame(false);
    EnableImageProcessingDialog(false);
    EnableTab(false);
    ClearImageInterfaceUI();
}






/**********************************************************
 * IMAGING
 * */


 bool FEniCS_Blood_Sim::eventFilter(QObject *obj, QEvent *event)
 {

     QVTKWidget * _qvtkWidget =  qobject_cast<QVTKWidget *>(obj);
     if (( _qvtkWidget)  && (event->type() == QEvent::MouseButtonPress))
     {
         QString _widgetName = _qvtkWidget->objectName();

         if (QString::compare(_widgetName,ui->axialViewWidget->objectName(),Qt::CaseSensitive) == StrEqual)
             LoadAxialImage();
         if (QString::compare(_widgetName,ui->sagittalViewWidget->objectName(),Qt::CaseSensitive) == StrEqual)
            LoadSaggitalImage();
         if (QString::compare(_widgetName,ui->coronalViewWidget->objectName(),Qt::CaseSensitive) == StrEqual)
            LoadCoronalImage();

         return true;
     } else {
         // standard event processing
         return QObject::eventFilter(obj, event);
     }
 }

 void FEniCS_Blood_Sim::ClearImageInterfaceUI()
 {
     if (this->ui->mainImageWidget->GetRenderWindow()->HasRenderer(mainImRendererTab))
     {
         mainImRendererTab->Clear();
         this->ui->mainImageWidget->GetRenderWindow()->RemoveRenderer(mainImRendererTab);
         this->ui->mainImageWidget->GetRenderWindow()->ClearInRenderStatus();
     }else
         ui->mainImageWidget->setEnabled(false);

      ui->axialViewWidget->setEnabled(false);
      ui->coronalViewWidget->setEnabled(false);
      ui->sagittalViewWidget->setEnabled(false);
  }

#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
 #include "vtkImageMapper3D.h"

#include "vtkMetaImageReader.h"
#include "vtkImageSliceMapper.h"

 void FEniCS_Blood_Sim::LoadMainImageTab()
 {
     if (!ui->mainImageWidget->isEnabled())
         ui->mainImageWidget->setEnabled(true);

    mainImRendererTab->SetBackground(0,0,0);




    // read input image
    vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    reader->SetFileName(_projectData->getImPath().toStdString().c_str());
    reader->Update();

    // Visualize
    vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
    imageSliceMapper->SetInputData(reader->GetOutput());

    vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
    imageSlice->SetMapper(imageSliceMapper);

    // Setup renderers
    mainImRendererTab->AddViewProp(imageSlice);
    mainImRendererTab->ResetCamera();
    ui->mainImageWidget->GetRenderWindow()->AddRenderer(mainImRendererTab);


 }


 void FEniCS_Blood_Sim::LoadAxialImage()
 {
    // TODO

     axialImRendererTab->SetBackground(0,0,0);
     ui->axialViewWidget->GetRenderWindow()->AddRenderer(axialImRendererTab);
 }
 void FEniCS_Blood_Sim::LoadSaggitalImage()
 {
     // TODO
    saggitalImRendererTab->SetBackground(0,0,0);
    ui->sagittalViewWidget->GetRenderWindow()->AddRenderer(saggitalImRendererTab);

 }
 void FEniCS_Blood_Sim::LoadCoronalImage()
 {
     // TODO
     coronalImRendererTab->SetBackground(0,0,0);
     ui->coronalViewWidget->GetRenderWindow()->AddRenderer(coronalImRendererTab);

 }
 void FEniCS_Blood_Sim::LoadImageInterfaceUI(const QString imPath)
 {
    if (imPath.isEmpty()) return;
     // Loading image
    _projectData->setImPath(imPath);
    _projectData->loadImData();

    // Updating the UI
    LoadMainImageTab();
    LoadAxialImage();
    LoadSaggitalImage();
    LoadCoronalImage();

 }


