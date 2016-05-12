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


void GenericWorkflowDialog::on_tableMethods1_cellClicked(int row, int column)
{
    if (column == CHECKABLE_COLUMN)
        loadIcosn(row);


}

void GenericWorkflowDialog::loadIcosn(int row)
{
    _dragableArea->insertItem(row);
}

void GenericWorkflowDialog::createTabWithName(int tabIndex, const QString text)
{
    if ((tabIndex == 0) || (tabIndex == 1))
        ui->tabMethods->setTabText(tabIndex,text);
    else
    {
        QWidget *newTab = new QWidget(ui->tabMethods);
        ui->tabMethods->addTab(newTab, text);
    }
}

#include <QTableWidget>
#include <QHBoxLayout>

void GenericWorkflowDialog::on_tabMethods_currentChanged(int index)
{
    // Insert Tablewidget
    //int cIndex = ui->tabMethods->currentIndex();
    QWidget * _cWidget = ui->tabMethods->currentWidget();


    QTableWidget * _table = new QTableWidget();
    _table->insertRow(0);
    _table->insertRow(1);
    QHBoxLayout *l = new QHBoxLayout(_cWidget);
    l->addWidget(_table);


}
