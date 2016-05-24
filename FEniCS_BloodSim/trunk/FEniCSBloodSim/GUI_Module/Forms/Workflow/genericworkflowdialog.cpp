#include "genericworkflowdialog.h"
#include "ui_genericworkflowdialog.h"

#include <QTableWidgetItem>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QObject>


#include "GUI_Module/Defines/Menu/MenuDefines.h"

GenericWorkflowDialog::GenericWorkflowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenericWorkflowDialog)
{
    ui->setupUi(this);
    _cPipelineRow = -1;
    _wkfHelper  = new WorkflowTableHelper();
    _wkfHelper->registerTableListUI(ui->tableMethods1);
    _wkfHelper->setCurrentRegisteredTableFromList(0);

    _pipelineHelper = new WorkflowTableHelper();
    _pipelineHelper->registerTableUI(ui->pipelineTable);

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
        updateValuesPipelineTable(row,column);
}

void GenericWorkflowDialog::updateValuesPipelineTable(int row,int column)
{

    QList<CategoryWkfData> _catList = _wkfData.getCategoryList();
    int _cTab = ui->tabMethods->currentIndex();
    if (_cTab < _catList.size())
    {
        CategoryWkfData _cCategory = _catList.at(_cTab);
        QString _iconPath = _cCategory.getIconPath();
        QString _description = buildDescription(_cCategory,row);
        if (_cPipelineRow == -1)
            return;
        _pipelineHelper->updateRow(_iconPath,_description,_cPipelineRow);
        updatePipelineElement(_iconPath, _description, _cCategory.getCategoryName());
    }

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



void GenericWorkflowDialog::on_tabMethods_currentChanged(int index)
{
    // Insert Tablewidget
    fillTableWithInformation(index);  
}

void GenericWorkflowDialog::fillTableWithInformation(int index)
{
    QWidget * _cWidget = ui->tabMethods->currentWidget();
    QList<CategoryWkfData> _catList = _wkfData.getCategoryList();
    if (!_wkfHelper->isRegistered(index))
    {

        QTableWidget * _table = new QTableWidget();
        _table->insertColumn(CHECKABLE_COLUMN);
        _table->insertColumn(ONLY_DESC_COLUMN);

        _wkfHelper->registerTableListUI(_table);
        _wkfHelper->setCurrentRegisteredTableFromList(index);

        if (index < _catList.size())
        {
            CategoryWkfData _catSelected = _catList.at(index);
            QList<ImagingWkfFunctionData> _imgList = _catSelected.getListFunctions();

            for (int i = 0; i< _imgList.size();i++)
            {
                ImagingWkfFunctionData cFunction = _imgList.at(i);
                _wkfHelper->addOnlyDesc(cFunction.getName());
            }
        }

        connect(_table, SIGNAL(cellClicked(int,int)), this, SLOT(on_tableMethods1_cellClicked(int,int)));

        QHBoxLayout *l = new QHBoxLayout(_cWidget);
        l->addWidget(_table);

    }
}



void GenericWorkflowDialog::on_addStepToPipelineButton_clicked()
{
    insertRow();
    enableNextStep(true);


}
void GenericWorkflowDialog::insertRow()
{
    _cPipelineRow =  _pipelineHelper->addEmptyRow();
}



void GenericWorkflowDialog::enableNextStep(bool _val)
{
    ui->selectOperationStep->setEnabled(_val);
    ui->tabMethods->setEnabled(_val);
    ui->configOptionsTitle->setEnabled(_val);
    ui->optionsConfigFrame->setEnabled(_val);
    ui->stepDoneButton->setEnabled(_val);


    ui->addStepToPipelineButton->setEnabled(!_val);
    ui->pipelineTable->setEnabled(!_val);
}




QString GenericWorkflowDialog::buildDescription(CategoryWkfData _cCategory, int _row)
{
    QString _text = "";
    if (_cCategory.isEmpty())
        return _text;

    QList<ImagingWkfFunctionData> tempFunctionList = _cCategory.getListFunctions();
    if (_row < tempFunctionList.size())
    {
        ImagingWkfFunctionData _selFunction= tempFunctionList.at(_row);
         _text =   _selFunction.getName();
    }

    return _text;

}




void GenericWorkflowDialog::on_stepDoneButton_clicked()
{
    enableNextStep(false);
    restoreUI();
}

void GenericWorkflowDialog::updatePipelineElement(QString _iconPath, QString _description, QString _categoryName)
{
    int _cListSize =  _pipelineItemList.size() -1;

    if (_cPipelineRow <= _cListSize)
    {
       PipelineItem _item = _pipelineItemList.at(_cPipelineRow);
       _item.setIconPath(_iconPath);
       _item.setDescription(_description);
       _item.setCategoryName(_categoryName);
       _pipelineItemList.replace(_cPipelineRow,_item);
    }
    if (_cPipelineRow > _cListSize)
    {
        PipelineItem _item;
        _item.setIconPath(_iconPath);
        _item.setDescription(_description);
        _item.setCategoryName(_categoryName);
        _pipelineItemList.append(_item);
    }


}



void GenericWorkflowDialog::restoreUI()
{
    // TODO
}





void GenericWorkflowDialog::on_pipelineTable_cellClicked(int row, int column)
{
    switch (column) {
    case UP_ICON_COLUMN:
        moveUp(row);
        break;
    case DOWN_ICON_COLUMN:
        moveDown(row);
        break;
    case DELETE_ICON_COLUMN:
        ui->pipelineTable->removeRow(row);
        _pipelineItemList.removeAt(row);
        break;
    default:
        break;
    }

}

void GenericWorkflowDialog::moveDown(int row)
{
     if (row == ui->pipelineTable->rowCount()-1) return;

     int _nextRow = row+1;
     PipelineItem _nextItem = _pipelineItemList.at(_nextRow);
     PipelineItem _cItem = _pipelineItemList.at(row);

     // Changing the content of the table view
     _pipelineHelper->updateRow(_cItem.getIconPath(),_cItem.getDescription(), _nextRow);
     _pipelineHelper->updateRow(_nextItem.getIconPath(),_nextItem.getDescription(),row);

     // Update the Pipeline list

     _pipelineItemList.replace(_nextRow, _cItem);
     _pipelineItemList.replace(row, _nextItem);

}


void GenericWorkflowDialog::moveUp(int row)
{
     if (row == 0) return;


     int _previousRow = row-1;
     PipelineItem _previousItem = _pipelineItemList.at(_previousRow);
     PipelineItem _cItem = _pipelineItemList.at(row);

     // Changing the content of the table view
     _pipelineHelper->updateRow(_cItem.getIconPath(),_cItem.getDescription(), _previousRow);
     _pipelineHelper->updateRow(_previousItem.getIconPath(),_previousItem.getDescription(),row);

     // Update the Pipeline list

     _pipelineItemList.replace(_previousRow, _cItem);
     _pipelineItemList.replace(row, _previousItem);

}

//------------------------------------------
// DRAG & DROP FUNCTIONS
// FOr the future, not now
/*void GenericWorkflowDialog::loadIcon(int row)
{
    QList<CategoryWkfData> _catList = _wkfData.getCategoryList();
    int _cTab = ui->tabMethods->currentIndex();
    if (_cTab < _catList.size())
    {
        CategoryWkfData _cCategory = _catList.at(_cTab);
        QString _iconPath = _cCategory.getIconPath();
        QString _description = buildDescription(_cCategory,row);
        _dragableArea->insertItem(_iconPath,_description);
    }
}*/
