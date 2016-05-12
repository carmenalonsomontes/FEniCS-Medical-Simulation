#include "genericworkflowdialog.h"
#include "ui_genericworkflowdialog.h"



#include "GUI_Module/Defines/Menu/MenuDefines.h"

GenericWorkflowDialog::GenericWorkflowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenericWorkflowDialog)
{
    ui->setupUi(this);
    ui->tableMethods1->setDropIndicatorShown(true);
    ui->tableMethods1->setDragEnabled(true);     

    _wkfHelper  = new WorkflowTableHelper();
    _wkfHelper->registerTableUI(ui->tableMethods1);

    // Dragable Area
    _dragableArea = new DragItem(this);
     ui->wkfFrame->setAcceptDrops(true);
     ui->wkfFrame->layout()->addWidget(_dragableArea);

}

GenericWorkflowDialog::~GenericWorkflowDialog()
{
    delete ui;
}



// =========================================================================
// Accepted BUTTONBOX
// =========================================================================
void GenericWorkflowDialog::on_wkfButtonBox_accepted()
{
    close();
}
// =========================================================================
// General Logic
// =========================================================================

 void GenericWorkflowDialog::loadDragableMethods()
 {
     _wkfHelper->addOnlyDesc("kk");
     _wkfHelper->addOnlyDesc("kk2");

 }

//#include "itkCannyEdgeDetectionImageFilter.h"

 /*void GenericWorkflowDialog::createProgram()
 {
     //std::string filterName= "itkCannyEdgeDetectionImageFilter";

 }*/

void GenericWorkflowDialog::on_tableMethods1_cellClicked(int row, int column)
{
    if (column == CHECKABLE_COLUMN)
        loadIcosn(row);


}

void GenericWorkflowDialog::loadIcosn(int row)
{
    _dragableArea->insertItem(row);
}




