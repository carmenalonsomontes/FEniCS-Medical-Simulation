#include "genericworkflowdialog.h"
#include "ui_genericworkflowdialog.h"

#include "GUI_Module/Defines/Menu/MenuDefines.h"

#include <QTableWidgetItem>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QObject>

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
        loadIcon(row);
}



void GenericWorkflowDialog::loadIcon(int row)
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
}

QString GenericWorkflowDialog::buildDescription(CategoryWkfData _cCategory, int _row)
{
    QString _text = "";
    _text = "Category: " + _cCategory.getCategoryName() + "\n";
    _text = _text + "Function: " + "Row:"+ QString::number(_row);
    return _text;

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
