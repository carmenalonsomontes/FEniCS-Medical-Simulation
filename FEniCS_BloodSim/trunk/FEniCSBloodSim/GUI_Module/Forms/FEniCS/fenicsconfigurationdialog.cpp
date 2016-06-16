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

}

void FEniCSConfigurationDialog::on_cancelConfigButton_clicked()
{

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
void FEniCSConfigurationDialog::on_firstTabTable_cellClicked(int row, int column)
{
    if (column == FENICS_ACTION_COLUMN)
    {
        // TODO
        insertConfigurationOptions(row,column);

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
        QString _parameterMethodName = _parameter.getMethodName();
        QString _parameterDefaultValue = _parameter.getDefaultValue();

        _fenicsOptionsTableHelper->addParameterRow(_parameterName,"",_parameterMethodName,_parameterDefaultValue);

    }
}
