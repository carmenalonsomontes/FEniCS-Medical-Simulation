#include "imageworkflow.h"

//#include "GUI_Module/DragItem/dragitem.h"
#include "GUI_Module/XML/xmlworkflowmethodsdata.h"
#include "GUI_Module/Defines/Workflow/WorkflowDefines.h"

#include <QList>
#include <QWidget>


ImageWorkflow::ImageWorkflow()
{
    // Loading the menu
    XMLWorkflowMethodsData _xmlReader;
    _xmlReader.readWorkflowMethodsFile(_wkfData,IMAGING_WKF);

    loadInitialMenu();

}


void ImageWorkflow::setImagePath(QString imPath)
{
    _wkfData->setImagePath(imPath);
}


void ImageWorkflow::loadInitialMenu()
{
    if (_wkfData->isEmpty())
        return;
    QList<CategoryWkfData> _catList = _wkfData->getCategoryList();

    if (_catList.isEmpty())
        return;

    // Create tabs & Update names
    fillWorkflowTabInfo(_catList);

    // Registering the rest of the tables
    for (int i=1;i<_catList.size();i++)
        fillTableWithInformation(i);


}



void ImageWorkflow::fillWorkflowTabInfo(QList<CategoryWkfData> catList)
{
    // Creating the tabs
    createCategoryTabs(catList.size(),catList);

    // Fill the table of the first Tab
    int _tabIndex = 0;
    fillMenuTable(catList.at(_tabIndex));
}



void ImageWorkflow::createCategoryTabs(int noTabs,QList<CategoryWkfData> catList)
{

    for (int i= 0; i< noTabs; i++)
    {
        CategoryWkfData _catData = catList.at(i);
        createTabWithName(i,_catData.getCategoryName());

    }

}


void ImageWorkflow::fillMenuTable(CategoryWkfData _catObj)
{
    QList<ImagingWkfFunctionData> _functList =  _catObj.getListFunctions();
    if (_functList.isEmpty())
        return;
    for (int i = 0; i< _functList.size();i++)
    {
        ImagingWkfFunctionData _functionObj= _functList.at(i);
        _wkfHelper->addMethodsAndAction(_functionObj.getName(),ADD_ICON);
    }
}



void ImageWorkflow::loadSpecificMethods()
{

}
