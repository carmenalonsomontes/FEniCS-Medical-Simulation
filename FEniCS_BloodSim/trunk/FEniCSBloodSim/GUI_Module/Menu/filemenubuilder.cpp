#include "filemenubuilder.h"
#include <QFileDialog>
#include <QCoreApplication>
FileMenuBuilder::FileMenuBuilder()
{
}

void FileMenuBuilder::launchMenuAction(int _action)
{
    int _successfunc = SUCCESS;
    bool _updateUI = true;
    switch(_action)
    {
    case NEW_PROJECT:
        // If previous project is open --> Close it
        if (!_registered_project_data->isEmpty())
            launchMenuAction(CLOSE_PROJECT);
        _successfunc = createNewProject();

        break;
    case OPEN_PROJECT:
        // If previous project is open --> Close it
        if (!_registered_project_data->isEmpty())
            launchMenuAction(CLOSE_PROJECT);
        _successfunc = openProject();

        break;
    case SAVE_PROJECT:
        _registered_project_data->saveProjectInfoToFile();
        break;
    case SAVEAS_PROJECT:
        _updateUI = saveas_project();
        break;
    case CLOSE_PROJECT:
        // Save data of the project
        _registered_project_data->saveProjectInfoToFile();
        // Clear data structure
        _registered_project_data->clearData();
        break;
    }

    if (_successfunc == FAILURE) return;
    if (!_updateUI) return;
    // Updating UI
    updateUI(_action);
    enableUIElements(_action);

}


void FileMenuBuilder::launchMenuAction(int _action,QString _pathfile)
{
    int _successfunc = SUCCESS;
    bool _updateUI = true;
    switch(_action)
    {
    case OPEN_PROJECT:
        // If previous project is open --> Close it
        if (!_registered_project_data->isEmpty())
            launchMenuAction(CLOSE_PROJECT);
        _successfunc = openProject(_pathfile);

        break;
    }

    if (_successfunc == FAILURE) return;
    if (!_updateUI) return;
    // Updating UI
    updateUI(_action);
    enableUIElements(_action);

}



int FileMenuBuilder::createNewProject()
{
    // Ask for path and project name
    QString projectPath = QFileDialog::getSaveFileName(_registered_window_parent, "Create Project File",QCoreApplication::applicationDirPath(),
                               "Fenics Blood Sim Project (*."+FBS_FILE_PROJECT_EXT+")");

    // Create basic structure
    if ((projectPath.isEmpty()) || (projectPath.isNull()))   return FAILURE;

    // Save Project data in memory
    _registered_project_data->saveProjectPathName(projectPath);

    // SAVE BASIC XML PROJECT INFO
    _registered_project_data->saveProjectInfoToFile();
    emit updateRecentProjectList(projectPath);

    return SUCCESS;
}

int FileMenuBuilder::openProject()
{
    // Now we can open the new project
    QString projectPath = QFileDialog::getOpenFileName(_registered_window_parent, "Create Project File",QCoreApplication::applicationDirPath(),
                                                       "Fenics Blood Sim Project (*."+FBS_FILE_PROJECT_EXT+")");
    return openProject(projectPath);
 }



int FileMenuBuilder::openProject(QString projectPath)
{
     // Create basic structure
    if ((projectPath.isEmpty()) || (projectPath.isNull()))  return FAILURE;

   // Load data in memory
    _registered_project_data->readProjectInfoFromFile(projectPath);
    emit updateRecentProjectList(projectPath);

    return SUCCESS;
}



bool FileMenuBuilder::saveas_project()
{

    // Ask for path and project name
    QString projectPath = QFileDialog::getSaveFileName(_registered_window_parent, "SAVE Project File AS",QCoreApplication::applicationDirPath(),
                               "Fenics Blood Sim Project (*."+FBS_FILE_PROJECT_EXT+")");

    // Create basic structure
    if ((projectPath.isEmpty()) || (projectPath.isNull()))   return false;

    // Save Project data in memory
    _registered_project_data->saveProjectPathName(projectPath);

    // SAVE BASIC XML PROJECT INFO
    _registered_project_data->saveProjectInfoToFile();
    emit updateRecentProjectList(projectPath);

    return true;
}


//---------------------------------------------------------------------------------------
/** UI ELEMENTS */

void FileMenuBuilder::updateUI(int _menu)
{
    // Update StatusBar
    if ((_menu == NEW_PROJECT) || (_menu == OPEN_PROJECT) || (_menu == CLOSE_PROJECT)
            || (_menu == SAVEAS_PROJECT))
        emit updateStatusBarUI(_registered_project_data->getProjectName());

}

void FileMenuBuilder::enableUIElements(int _menu)
{
    bool _valClose = false;
    bool _valSave = false;
    if ((_menu == NEW_PROJECT) || (_menu == OPEN_PROJECT))
    {
        _valClose = true;
        _valSave = true;
    }
    if ((_menu == SAVE_PROJECT) || (_menu == SAVEAS_PROJECT))
        _valClose = true;

    emit enableCloseProjectUI(_valClose);
    emit enableSaveProjectUI(_valSave);
}
