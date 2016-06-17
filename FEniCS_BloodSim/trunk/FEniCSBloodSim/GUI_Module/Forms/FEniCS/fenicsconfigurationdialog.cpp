#include "fenicsconfigurationdialog.h"
#include "ui_fenicsconfigurationdialog.h"

#include <QWidget>
#include <QHBoxLayout>


#include "GUI_Module/XML/xmlfenicsinformationdata.h"
#include "GUI_Module/Defines/FEniCS/FEniCSDefines.h"

FEniCSConfigurationDialog::FEniCSConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FEniCSConfigurationDialog)
{
    ui->setupUi(this);
    _fenicsData = new FEniCSXMLData();
    initializeHelpers();
    loadFenicsData();


    _userAcceptChanges = false;
    _cPipelineRow = -1;
    _selectedRow = -1;
}

FEniCSConfigurationDialog::~FEniCSConfigurationDialog()
{
    delete ui;
    delete _fenicsTableTabeHelper;
    delete _fenicsOptionsTableHelper;
    delete _fenicsPipelineTableHelper;
    delete _fenicsPipelineOptionsTableHelper;
}

bool FEniCSConfigurationDialog::userAcceptChanges()
{
    return _userAcceptChanges;
}



void FEniCSConfigurationDialog::initializeHelpers()
{
    _fenicsTableTabeHelper = new WorkflowTableHelper();
    _fenicsTableTabeHelper->registerTableListUI(ui->firstTabTable);
    _fenicsTableTabeHelper->setCurrentRegisteredTableFromList(0);


    _fenicsOptionsTableHelper = new WorkflowTableHelper();
    _fenicsOptionsTableHelper->registerTableUI(ui->fenicsOptionsTable);

    _fenicsPipelineTableHelper = new WorkflowTableHelper();
    _fenicsPipelineTableHelper->registerTableUI(ui->fenicsPipelineTable);

    _fenicsPipelineOptionsTableHelper = new WorkflowTableHelper();
    _fenicsPipelineOptionsTableHelper->registerTableUI(ui->fenicsPipelineOptionsTable);

}

// -------------------------------------------------------------------
// BUTTON BOX
void FEniCSConfigurationDialog::on_dialogButtonBox_accepted()
{
    _userAcceptChanges = true;
    close();
}

void FEniCSConfigurationDialog::on_dialogButtonBox_rejected()
{
    _userAcceptChanges = false;
    close();
}
// -------------------------------------------------------------------


// Configuration BUTTONS
void FEniCSConfigurationDialog::on_stepDoneButton_clicked()
{

    // Adding the info to the pipeline
    insertRow();
    QList<CategoryFEniCSData> _catList = _fenicsData->getCategoryList();
    int _cTab = ui->fenicsTabWidget->currentIndex();
    if (_cTab < _catList.size())
    {
        CategoryFEniCSData _cCategory = _catList.at(_cTab);
        QString _description = buildDescription(_cCategory,_selectedRow);
        if (_cPipelineRow == -1)
            return;
        _fenicsPipelineTableHelper->updateRowWithCategoryName(_cCategory.getCategoryName(),_description,_cPipelineRow);
        updatePipelineElement(_description, _cCategory,_selectedRow);
    }
    enableNextStep(false);
    restoreUI();
   // saveConfiguration();

}

/*
void FEniCSConfigurationDialog::saveConfiguration()
{
    int _noParams = ui->fenicsOptionsTable->rowCount();
    if (_noParams == 0 ) return;

    QList<FEniCSParameterPipelineData> _parameterList;
    for (int i= 0; i < _noParams ; i++)
    {
        FEniCSParameterPipelineData _item;
        _item.set(ui->cPipelineConfigurationTable->item(i, PARAM_NAME_COLUMN)->text());
        _item.setOptionType(ui->cPipelineConfigurationTable->item(i, PARAM_TYPE_COLUMN)->text());
        _item.setCurrentValue(ui->cPipelineConfigurationTable->item(i, PARAM_VALUE_COLUMN)->text());
        _item.setMethodName(ui->cPipelineConfigurationTable->item(i, PARAM_METHOD_NAME_COLUMN)->text());
        _parameterList.append(_item);
    }
    int _lastPos = _pipelineItemList.size() -1;
    FEniCSPipelineData  _cPipelineItem = _pipelineItemList.at(_lastPos);
    _cPipelineItem.setConfigurationItemList(_parameterList);
    _pipelineData.replace(_lastPos,_cPipelineItem);
}
*/

void FEniCSConfigurationDialog::updatePipelineElement(QString _description, CategoryFEniCSData _category,int noFunction)
{
    FEniCSPipelineData  _item;
    int _cListSize =  _pipelineData.size() -1;

    if (_cPipelineRow <= _cListSize)
        _item = _pipelineData.at(_cPipelineRow);

    // -------------------------------------------------------------------

    // Category
    _item.setCategoryName(_category.getCategoryName());
    _item.setKeyInput(_category.getKeyInput());
    _item.setFunctionDesc(_description);

   // Parameters
   QList<FunctionFEniCSData> _functList = _category.getListFunctions();
   FunctionFEniCSData _function = _functList.at(noFunction);
   _item.setFunctionName(_function.getFunctionName());
   _item.setFunctionFenicsName(_function.getFenicsFunctionName());
   _item.setFunctionDesc(_function.getDescription());

   QList<ParameterFEniCSData> _parameterList = _function.getParameterList();

   QList<FEniCSParameterPipelineData> _cParameterList;
   for (int i = 0; i < _parameterList.size();i++)
   {
       ParameterFEniCSData _parameter = _parameterList.at(i);
       FEniCSParameterPipelineData _parameterItem;

       _parameterItem.setParameterName(_parameter.getName());
       _parameterItem.setDefaultValue(_parameter.getDefaultValue());
       _parameterItem.setSelectedValue(ui->fenicsOptionsTable->item(i,FENICS_PARAM_VALUE)->text());
       _parameterItem.setDescription(_parameter.getDescription());
       _parameterItem.setMethodName(_parameter.getMethodName());
       _cParameterList.append(_parameterItem);
   }

   _item.setParameterPipelineItems(_cParameterList);


    if (_cPipelineRow <= _cListSize)
        _pipelineData.replace(_cPipelineRow,_item);
    if (_cPipelineRow > _cListSize)
        _pipelineData.append(_item);

 }



void FEniCSConfigurationDialog::on_cancelConfigButton_clicked()
{
     enableNextStep(false);
     restoreUI();

}
void FEniCSConfigurationDialog::restoreUI()
{
    // TODO
    _fenicsOptionsTableHelper->clearTable();
    _selectedRow = -1;

}



void FEniCSConfigurationDialog::insertRow()
{
    _cPipelineRow =  _fenicsPipelineTableHelper->addEmptyRow();
}

QString FEniCSConfigurationDialog::buildDescription(CategoryFEniCSData _cCategory, int _row)
{
    QString _text = "";
    if (_cCategory.isEmpty())
        return _text;

    QList<FunctionFEniCSData> tempFunctionList = _cCategory.getListFunctions();
    if (_row < tempFunctionList.size())
    {
        FunctionFEniCSData _selFunction= tempFunctionList.at(_row);
         _text =   _selFunction.getFunctionName();
    }

    return _text;

}



void FEniCSConfigurationDialog::enableNextStep(bool _val)
{

    ui->step2TitleLabel->setEnabled(_val);
    ui->stepDoneButton->setEnabled(_val);
    ui->cancelConfigButton->setEnabled(_val);
    ui->Step2Frame->setEnabled(_val);
}


// -------------------------------------------------------------------
void FEniCSConfigurationDialog::loadFenicsData()
{
    XmlFEniCSInformationData _xmlReader;

    _xmlReader.readFEniCSFile(_fenicsData);

    createCategoryTabs();
    QList<CategoryFEniCSData> _catList = _fenicsData->getCategoryList();

    fillMenuTable(_catList.at(0));

    for (int i=0;i < _catList.size();i++)
        fillTableWithInformation(i);
}

void FEniCSConfigurationDialog::createCategoryTabs()
{
    QList<CategoryFEniCSData> _catList = _fenicsData->getCategoryList();
    for (int i= 0; i < _catList.size();i++)
    {
        CategoryFEniCSData _currentCat = _catList.at(i);
        createTab(_currentCat,i);
    }
}


void FEniCSConfigurationDialog::createTab(CategoryFEniCSData category, int tabIndex)
{
    if (category.isEmpty()) return;
    QString _catName = category.getCategoryName();

    if ((tabIndex == 0) || (tabIndex == 1))
        ui->fenicsTabWidget->setTabText(tabIndex,_catName);
    else
    {
        QWidget *newTab = new QWidget(ui->fenicsTabWidget);
        ui->fenicsTabWidget->addTab(newTab, _catName);
    }

}

void FEniCSConfigurationDialog::fillMenuTable(CategoryFEniCSData _catObj)
{
    QList<FunctionFEniCSData> _functList =  _catObj.getListFunctions();
    if (_functList.isEmpty())
        return;
    for (int i = 0; i< _functList.size();i++)
    {
        FunctionFEniCSData _functionObj= _functList.at(i);
        _fenicsTableTabeHelper->addMethodsAndAction(_functionObj.getFunctionName(),ADD_ICON);
    }
}


void FEniCSConfigurationDialog::fillTableWithInformation(int index)
{
    QWidget * _cWidget = ui->fenicsTabWidget->widget(index);
    QList<CategoryFEniCSData> _catList = _fenicsData->getCategoryList();
    if (!_fenicsTableTabeHelper->isRegistered(index))
    {
        createTabTables(index);

        if (index < _catList.size())
        {
            CategoryFEniCSData _catSelected = _catList.at(index);
            QList<FunctionFEniCSData> _funcList = _catSelected.getListFunctions();

            for (int i = 0; i< _funcList.size();i++)
            {
                FunctionFEniCSData cFunction = _funcList.at(i);
                _fenicsTableTabeHelper->addMethodsAndAction(cFunction.getFunctionName(),ADD_ICON);
            }
        }

        connect(_fenicsTableTabeHelper->getRegisteredTable(), SIGNAL(cellClicked(int,int)), this, SLOT(on_firstTabTable_cellClicked(int,int)));

        QHBoxLayout *l = new QHBoxLayout(_cWidget);
        l->addWidget(_fenicsTableTabeHelper->getRegisteredTable());
    }
}
void FEniCSConfigurationDialog::createTabTables(int noTable)
{
    QTableWidget * _table = new QTableWidget();

    _table->insertColumn(FENICS_METHOD_NAME_COLUMN);
    _table->insertColumn(FENICS_ACTION_COLUMN);

    _table->setHorizontalHeaderLabels(QString("Operation;Action").split(";"));

    _fenicsTableTabeHelper->registerTableListUI(_table);
    _fenicsTableTabeHelper->setCurrentRegisteredTableFromList(noTable);
}


// STEP 1 -- TABS & TABLES

//--------------------------------------------------------------------------------------------
// CONFIGURATION PARAMETERS TABLE
void FEniCSConfigurationDialog::on_firstTabTable_cellClicked(int row, int column)
{
    if (column == FENICS_ACTION_COLUMN)
    {
        // TODO
        enableNextStep(true);
        insertConfigurationOptions(row,column);
        _selectedRow = row;
    }
}

void FEniCSConfigurationDialog::insertConfigurationOptions(int row,int column)
{

    QList<CategoryFEniCSData> _catList = _fenicsData->getCategoryList();
    int _cTab = ui->fenicsTabWidget->currentIndex();
    if (_cTab < _catList.size())
    {
        CategoryFEniCSData _cCategory = _catList.at(_cTab);
        addParametersToConfigurationTable(row,_cCategory);
    }

}

void FEniCSConfigurationDialog::addParametersToConfigurationTable(int row,CategoryFEniCSData cat)
{
   _fenicsOptionsTableHelper->clearTable();
   QList<FunctionFEniCSData> _functionList = cat.getListFunctions();
    FunctionFEniCSData _cFunction = _functionList.at(row);

    QList<ParameterFEniCSData> _parameterList = _cFunction.getParameterList();

    for (int i = 0; i <_parameterList.size();i++)
    {
        ParameterFEniCSData _parameter = _parameterList.at(i);
        QString _parameterName = _parameter.getName();       
        QString _parameterDefaultValue = _parameter.getDefaultValue();

        _fenicsOptionsTableHelper->addParameterRow(_parameterName,_parameterDefaultValue);

    }

}



//--------------------------------------------------------------------------------------------
// PIPELINE TABLE
void FEniCSConfigurationDialog::on_fenicsPipelineTable_cellClicked(int row, int column)
{
    _fenicsPipelineOptionsTableHelper->clearTable();
    switch (column) {
    case UP_ICON_COLUMN:
        moveUp(row);
        showParameterInformation(row);
        break;
    case DOWN_ICON_COLUMN:
        moveDown(row);
        showParameterInformation(row);
        break;
    case DELETE_ICON_COLUMN:
        ui->fenicsPipelineTable->removeRow(row);
       _pipelineData.removeAt(row);
        break;
    default:
        showParameterInformation(row);
        break;
    }
}

void FEniCSConfigurationDialog::moveUp(int row)
{
     if (row == 0) return;


     int _previousRow = row-1;
     FEniCSPipelineData  _previousItem = _pipelineData.at(_previousRow);
     FEniCSPipelineData  _cItem = _pipelineData.at(row);

     // Changing the content of the table view
     _fenicsPipelineTableHelper->updateRowWithCategoryName(_cItem.getCategoryName(),_cItem.getFunctionName(), _previousRow);
     _fenicsPipelineTableHelper->updateRowWithCategoryName(_previousItem.getCategoryName(),_previousItem.getFunctionName(),row);

     // Update the Pipeline list

     _pipelineData.replace(_previousRow, _cItem);
     _pipelineData.replace(row, _previousItem);

}
void FEniCSConfigurationDialog::moveDown(int row)
{
     if (row == ui->fenicsPipelineTable->rowCount()-1) return;

     int _nextRow = row+1;
     FEniCSPipelineData _nextItem = _pipelineData.at(_nextRow);
     FEniCSPipelineData _cItem = _pipelineData.at(row);

     // Changing the content of the table view
     _fenicsPipelineTableHelper->updateRowWithCategoryName(_cItem.getCategoryName(),_cItem.getFunctionDesc(), _nextRow);
     _fenicsPipelineTableHelper->updateRowWithCategoryName(_nextItem.getCategoryName(),_nextItem.getFunctionDesc(),row);

     // Update the Pipeline list

     _pipelineData.replace(_nextRow, _cItem);
     _pipelineData.replace(row, _nextItem);

}
void FEniCSConfigurationDialog::showParameterInformation(int row)
{
    if (_pipelineData.isEmpty())
        return;
    FEniCSPipelineData  _itemToShow = _pipelineData.at(row);

    QList<FEniCSParameterPipelineData> _itemList = _itemToShow.getParameterPipelineItems();
    for (int i = 0; i< _itemList.size();i++)
    {
        FEniCSParameterPipelineData _parameter = _itemList.at(i);
         _fenicsPipelineOptionsTableHelper->addParameterRow (_parameter.getParameterName(), _parameter.getSelectedValue());
    }

}


void FEniCSConfigurationDialog::on_fenicsPipelineOptionsTable_cellChanged(int row, int column)
{
    if (column != FENICS_PARAM_VALUE)
        return;

    int _selectedItem = ui->fenicsPipelineOptionsTable->currentRow();
    FEniCSPipelineData  _previousValuesItem = _pipelineData.at(_selectedItem);
    QList<FEniCSParameterPipelineData> _previousConfig = _previousValuesItem.getParameterPipelineItems();
    FEniCSParameterPipelineData _previousValue = _previousConfig.at(row);

    if (QString::compare(_previousValue.getSelectedValue(),ui->fenicsPipelineOptionsTable->item(row,column)->text()) != STR_EQUAL)
    {
        _previousValuesItem.updateParameterItemValue(ui->fenicsPipelineOptionsTable->item(row,column)->text(),row);
        _pipelineData.replace(_selectedItem,_previousValuesItem);
    }

}
