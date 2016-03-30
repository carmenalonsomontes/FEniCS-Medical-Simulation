#include "filemenubuilder.h"
#include <QFileDialog>
#include <QCoreApplication>
FileMenuBuilder::FileMenuBuilder()
{
}

void FileMenuBuilder::launchMenuAction(int _action)
{
    switch(_action)
    {
    case NEW_PROJECT:
        createNewProject();
        break;
    case OPEN_PROJECT:
        openProject();
        break;
    case RECENT_PROJECTS:
        break;
    case SAVE_PROJECT:
        break;
    case SAVEAS_PROJECT:
        break;
    case CLOSE_PROJECT:
        // Save data of the project
        // TODO

        // Clear data structure
        _registered_project_data->clearData();

        // Clean interface
        //TODO
        break;
    case EXIT_APP:
        _registered_window_parent->close();
        break;
    case EXIT_MAINWINDOW:
        _registered_window_parent->close();
        break;
    }

}

void FileMenuBuilder::createNewProject()
{
    // Ask for path and project name
    QString projectPath = QFileDialog::getSaveFileName(_registered_window_parent, "Create Project File",QCoreApplication::applicationDirPath(),
                               "Fenics Blood Sim Project (*."+FBS_FILE_PROJECT_EXT+")");

    // Create basic structure
    if ((projectPath.isEmpty()) || (projectPath.isNull()))   return;

    // Save Project data in memory
    _registered_project_data->saveProjectPathName(projectPath);

    // SAVE BASIC XML PROJECT INFO
    _registered_project_data->saveProjectInfoToFile();


}

void FileMenuBuilder::openProject()
{
    // If previous project is open --> Close it
    if (!_registered_project_data->isEmpty())
        launchMenuAction(CLOSE_PROJECT);

    // Now we can open the new project
    QString projectPath = QFileDialog::getOpenFileName(_registered_window_parent, "Create Project File",QCoreApplication::applicationDirPath(),
                                                       "Fenics Blood Sim Project (*."+FBS_FILE_PROJECT_EXT+")");
    // Create basic structure
    if ((projectPath.isEmpty()) || (projectPath.isNull()))   return;

   // Load data in memory
    _registered_project_data->readProjectInfoFromFile(projectPath);

}
