#include "genericworkflowdialog.h"
#include "ui_genericworkflowdialog.h"

#include "GUI_Module/Defines/Menu/MenuDefines.h"

#include <QTableWidgetItem>
#include <QTableWidget>
#include <QHBoxLayout>

GenericWorkflowDialog::GenericWorkflowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenericWorkflowDialog)
{
    ui->setupUi(this);
    ui->tableMethods1->setDropIndicatorShown(true);
    ui->tableMethods1->setDragEnabled(true);     

    _wkfHelper  = new WorkflowTableHelper();
    _wkfHelper->registerTableListUI(ui->tableMethods1);
    _wkfHelper->setCurrentRegisteredTableFromList(0);

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
        // Nota: necesito el signalMapper
        //connect(_table, SIGNAL(cellClicked(int,int)), _signalMapper, SLOT(map()),Qt::UniqueConnection);
        //_signalMapper->setMapping(_comboItem,QString("%1-%2").arg(i).arg(ColumnSelVal));
        //connect(_signalMapper, SIGNAL(mapped(const QString &)),this, SLOT(changedComboValueByUser(const QString &)),Qt::UniqueConnection);

        QHBoxLayout *l = new QHBoxLayout(_cWidget);
        l->addWidget(_table);

    }
}
void GenericWorkflowDialog::addSignals(int pos)
{




/*
    bool _foundCombos = false;
    bool _foundLineEdit = false;
    bool _foundCheckList = false;

    if (ui->solverVariablesTable->rowCount() > 0)
    {
        for (int i = 0; i< ui->solverVariablesTable->rowCount();i++)
        {
                QWidget * _item;
                _item = ui->solverVariablesTable->cellWidget(i,ColumnSelVal);

                if (_item)
                {
                    QComboBox *_comboItem = qobject_cast<QComboBox *> (_item);
                    if (_comboItem)
                    {
                        if (_comboItem->isEditable())
                        {
                            connect(_comboItem,SIGNAL(editTextChanged(QString)),_signalMapper,SLOT(map()),Qt::UniqueConnection);
                            _signalMapper->setMapping(_comboItem,QString("%1-%2").arg(i).arg(ColumnSelVal));
                        }
                        connect(_comboItem, SIGNAL(currentIndexChanged(int)), _signalMapper, SLOT(map()),Qt::UniqueConnection);
                       _signalMapper->setMapping(_comboItem, QString("%1-%2").arg(i).arg(ColumnSelVal));

                        _foundCombos = true;
                    }else
                    {
                        QLineEdit * _lineEdit = qobject_cast<QLineEdit *> (_item);
                        if (_lineEdit)
                        {
                            connect(_lineEdit,SIGNAL(editingFinished()),_signalMapperTxtEdit,SLOT(map()),Qt::UniqueConnection);
                            _signalMapperTxtEdit->setMapping(_lineEdit,QString("%1").arg(i));
                            _foundLineEdit = true;
                        }
                        else
                        {
                            QListWidget * _checkList = qobject_cast<QListWidget *> (_item);
                            if (_checkList)
                            {
                                connect(_checkList,SIGNAL(itemClicked(QListWidgetItem*)),_signalMapperCheckList,SLOT(map()),Qt::UniqueConnection);
                               _signalMapperCheckList->setMapping(_checkList,QString("%1").arg(i));
                               _foundCheckList = true;
                            }
                        }
                    }
                }
        }
    }
    if (_foundCombos)
        connect(_signalMapper, SIGNAL(mapped(const QString &)),this, SLOT(changedComboValueByUser(const QString &)),Qt::UniqueConnection);
    if (_foundLineEdit)
        connect(_signalMapperTxtEdit, SIGNAL(mapped(const QString &)),this, SLOT(textChangedSlot(const QString &)),Qt::UniqueConnection);
    if (_foundCheckList)
        connect(_signalMapperCheckList, SIGNAL(mapped(const QString &)),this, SLOT(handleItem(const QString &)),Qt::UniqueConnection);
        */
}
