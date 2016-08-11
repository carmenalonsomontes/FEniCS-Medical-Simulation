#include "genericworkflowdialog.h"
#include "ui_genericworkflowdialog.h"

#include <QTableWidgetItem>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QObject>
#include <QList>
#include <QDir>

#include "GUI_Module/Defines/Menu/MenuDefines.h"

#include "Pipeline/configurationpipelineitem.h"
#include "GUI_Module/Pipeline/ItkPipeline/ioperation.h"
#include "GUI_Module/Pipeline/ItkPipeline/operationfactory.h"

GenericWorkflowDialog::GenericWorkflowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenericWorkflowDialog)
{
    ui->setupUi(this);
    _cPipelineRow = -1;

    // One helper per each table
    _wkfHelper  = new WorkflowTableHelper();
    _wkfHelper->registerTableListUI(ui->tableMethods1);
    _wkfHelper->setCurrentRegisteredTableFromList(0);

    _pipelineHelper = new WorkflowTableHelper();
    _pipelineHelper->registerTableUI(ui->pipelineTable);

    _configurationHelper = new WorkflowTableHelper();
    _configurationHelper->registerTableUI(ui->cPipelineConfigurationTable);

    _summaryHelper = new WorkflowTableHelper();
    _summaryHelper->registerTableUI(ui->pipelineItemSummaryTable);

    _wkfData = new WorkflowData();


    _userImage = new ImageData();

    _userAcceptChanges = false;
    _selectedRow = -1;
    _wkfExecuted = false;



}

GenericWorkflowDialog::~GenericWorkflowDialog()
{
    delete ui;
    delete _wkfHelper;
    delete _pipelineHelper;
    delete _configurationHelper;
    delete _summaryHelper;
    delete _wkfData;
    //_userImage->clear();

}



// =========================================================================
// General Logic
// =========================================================================

bool GenericWorkflowDialog::userAcceptChanges()
{
    return _userAcceptChanges;
}

QList<PipelineItem> GenericWorkflowDialog::getPipelineItemList()
{
    return _pipelineItemList;
}
void GenericWorkflowDialog::setUserImageData(ImageData * _image)
{
    _userImage = _image;
}

void GenericWorkflowDialog::setUserProjectPath(QString _path)
{
    _projectPath = _path;
    _imgProjectPath = createImageResultFolder();
}

void GenericWorkflowDialog::setPipelineItemList(QList<PipelineItem> _list)
{
    if (_list.isEmpty())
        return;
    _pipelineItemList = _list;
    loadUserData();
    ui->runPipelineButton->setEnabled(true);
}

void GenericWorkflowDialog::loadUserData()
{
    for (int i = 0; i < _pipelineItemList.size();i++)
    {
        PipelineItem _item = _pipelineItemList.at(i);
        insertRow();
        _pipelineHelper->updateRow(_item.getIconPath(),_item.getDescription(),_cPipelineRow);
    }
}

void GenericWorkflowDialog::restoreUI()
{
    // TODO
    _configurationHelper->clearTable();
    _selectedRow = -1;

}

QString GenericWorkflowDialog::createImageResultFolder()
{

    QDir dir(_projectPath);
    dir.mkdir(TEMP_FOLDER_WKF_IMAGES);

    return QDir(_projectPath).filePath(TEMP_FOLDER_WKF_IMAGES);

}

void GenericWorkflowDialog::enableNextStep(bool _val)
{

    ui->configOptionsTitle->setEnabled(_val);
    ui->optionsConfigFrame->setEnabled(_val);
    ui->stepDoneButton->setEnabled(_val);
    ui->cancelSelection->setEnabled(_val);
    ui->cPipelineConfigurationTable->setEnabled(_val);


    ui->selectOperationStep->setEnabled(!_val);
    ui->tabMethods->setEnabled(!_val);
    ui->runPipelineButton->setEnabled(!_val);
}

// Save the changes of the user during the configuration phase
void GenericWorkflowDialog::saveConfiguration()
{
    int _noParams = ui->cPipelineConfigurationTable->rowCount();
    if (_noParams == 0 ) return;

    QList<ConfigurationPipelineItem> _parameterList;
    for (int i= 0; i < _noParams ; i++)
    {
        ConfigurationPipelineItem _item;
        _item.setOptionName(ui->cPipelineConfigurationTable->item(i, PARAM_NAME_COLUMN)->text());
        _item.setOptionType(ui->cPipelineConfigurationTable->item(i, PARAM_TYPE_COLUMN)->text());
        _item.setCurrentValue(ui->cPipelineConfigurationTable->item(i, PARAM_VALUE_COLUMN)->text());
        _item.setMethodName(ui->cPipelineConfigurationTable->item(i, PARAM_METHOD_NAME_COLUMN)->text());
        _parameterList.append(_item);
    }
    int _lastPos = _pipelineItemList.size() -1;
    PipelineItem  _cPipelineItem = _pipelineItemList.at(_lastPos);
    _cPipelineItem.setConfigurationItemList(_parameterList);
    _pipelineItemList.replace(_lastPos,_cPipelineItem);
}



// If the pipeline item is changed --> We need to save the item
void GenericWorkflowDialog::on_pipelineItemSummaryTable_cellChanged(int row, int column)
{
    if (column != PARAM_VALUE_COLUMN)
        return;

    int _selectedItem = ui->pipelineTable->currentRow();
    PipelineItem  _previousValuesItem = _pipelineItemList.at(_selectedItem);
    QList<ConfigurationPipelineItem> _previousConfig = _previousValuesItem.getConfigurationItemList();
    ConfigurationPipelineItem _previousValue = _previousConfig.at(row);

    if (QString::compare(_previousValue.getCurrentValue(),ui->pipelineItemSummaryTable->item(row,column)->text()) != STR_EQUAL)
    {
        _previousValuesItem.updateConfiguredItemValue(ui->pipelineItemSummaryTable->item(row,column)->text(),row);
        _pipelineItemList.replace(_selectedItem,_previousValuesItem);
    }

}

// =========================================================================
// TABLE METHODS
// =========================================================================


// =========================================================================
// TAB METHODS

void GenericWorkflowDialog::on_tableMethods1_cellClicked(int row, int column)
{
    if (column == ACTION_COLUMN)
    {
        //insertRow();
        enableNextStep(true);
        updateValuesPipelineTable(row,column);
        _selectedRow = row;
    }
}

void GenericWorkflowDialog::updateValuesPipelineTable(int row,int column)
{

    QList<CategoryWkfData> _catList = _wkfData->getCategoryList();
    int _cTab = ui->tabMethods->currentIndex();
    if (_cTab < _catList.size())
    {
        CategoryWkfData _cCategory = _catList.at(_cTab);
        addParametersToConfigurationTable(row,_cCategory);
    }

}


void GenericWorkflowDialog::addParametersToConfigurationTable(int row,CategoryWkfData cat)
{
    _configurationHelper->clearTable();
    QList<ImagingWkfFunctionData> _functionList = cat.getListFunctions();
    ImagingWkfFunctionData _cFunction = _functionList.at(row);

    QStringList _parameterNameList = _cFunction.getParametersName();
    QStringList _parameterTypeList = _cFunction.getParameterType();
    QStringList _parameterMethodNameList = _cFunction.getParameterMethodName();
    QStringList _parameterDefaultValueList = _cFunction.getDefaultValue();
    for (int i = 0; i <_parameterNameList.size();i++)
    {
        QString _parameterName = _parameterNameList.at(i);
        QString _parameterType = _parameterTypeList.at(i);
        QString _parameterMethodName = _parameterMethodNameList.at(i);
        QString _parameterDefaultValue = _parameterDefaultValueList.at(i);

        _configurationHelper->addParameterRow(_parameterName,_parameterType,_parameterMethodName,_parameterDefaultValue);

    }
}

// =========================================================================
// TAB CREATION


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


void GenericWorkflowDialog::fillTableWithInformation(int index)
{
    QWidget * _cWidget = ui->tabMethods->widget(index);
    QList<CategoryWkfData> _catList = _wkfData->getCategoryList();
    if (!_wkfHelper->isRegistered(index))
    {
        createTabTables(index);

        if (index < _catList.size())
        {
            CategoryWkfData _catSelected = _catList.at(index);
            QList<ImagingWkfFunctionData> _imgList = _catSelected.getListFunctions();

            for (int i = 0; i< _imgList.size();i++)
            {
                ImagingWkfFunctionData cFunction = _imgList.at(i);
                _wkfHelper->addMethodsAndAction(cFunction.getName(),ADD_ICON);
            }
        }

        connect(_wkfHelper->getRegisteredTable(), SIGNAL(cellClicked(int,int)), this, SLOT(on_tableMethods1_cellClicked(int,int)));

        QHBoxLayout *l = new QHBoxLayout(_cWidget);
        l->addWidget(_wkfHelper->getRegisteredTable());
    }
}

void GenericWorkflowDialog::createTabTables(int noTable)
{
    QTableWidget * _table = new QTableWidget();

    _table->insertColumn(METHOD_NAME_COLUMN);
    _table->insertColumn(ACTION_COLUMN);

    _table->setHorizontalHeaderLabels(QString("Operation;Action").split(";"));

    _wkfHelper->registerTableListUI(_table);
    _wkfHelper->setCurrentRegisteredTableFromList(noTable);
}


void GenericWorkflowDialog::insertRow()
{
    _cPipelineRow =  _pipelineHelper->addEmptyRow();
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




// =========================================================================
// PIPELINE TABLE

void GenericWorkflowDialog::updatePipelineElement(QString _iconPath, QString _description,CategoryWkfData _category,int noFunction)
{
    PipelineItem  _item;
    int _cListSize =  _pipelineItemList.size() -1;

    if (_cPipelineRow <= _cListSize) 
        _item = _pipelineItemList.at(_cPipelineRow);


   _item.setIconPath(_iconPath);
   _item.setDescription(_description);
   _item.setCategoryName(_category.getCategoryName());

   QList<ImagingWkfFunctionData> _functList = _category.getListFunctions();
   ImagingWkfFunctionData _function = _functList.at(noFunction);
   _item.setFunctionName(_function.getName());
   _item.setFunctionClassName(_function.getClassName());
   _item.setFunctionDescription(_function.getDescription());

    if (_cPipelineRow <= _cListSize)
        _pipelineItemList.replace(_cPipelineRow,_item);
    if (_cPipelineRow > _cListSize)
        _pipelineItemList.append(_item);

 }




void GenericWorkflowDialog::showParameterInformation(int row)
{
    if (_pipelineItemList.isEmpty())
        return;
    PipelineItem  _itemToShow = _pipelineItemList.at(row);

    QList<ConfigurationPipelineItem> _itemList = _itemToShow.getConfigurationItemList();
    for (int i = 0; i< _itemList.size();i++)
    {
        ConfigurationPipelineItem _parameter = _itemList.at(i);
         _summaryHelper->addParameterRow(_parameter.getOptionName(),_parameter.getOptionType(),
                                         _parameter.getMethodName(),_parameter.getCurrentValue());
    }

}






// **********************************************************************************+
// PIPELINE TABLE  --> ROWS

// =========================================================================
// MOVE DOWN
// =========================================================================

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

// =========================================================================
// MOVE UP
// =========================================================================

void GenericWorkflowDialog::moveUp(int row)
{
     if (row == 0) return;


     int _previousRow = row-1;
     PipelineItem  _previousItem = _pipelineItemList.at(_previousRow);
     PipelineItem  _cItem = _pipelineItemList.at(row);

     // Changing the content of the table view
     _pipelineHelper->updateRow(_cItem.getIconPath(),_cItem.getDescription(), _previousRow);
     _pipelineHelper->updateRow(_previousItem.getIconPath(),_previousItem.getDescription(),row);

     // Update the Pipeline list

     _pipelineItemList.replace(_previousRow, _cItem);
     _pipelineItemList.replace(row, _previousItem);

}


void GenericWorkflowDialog::on_pipelineTable_cellClicked(int row, int column)
{
    _summaryHelper->clearTable();
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
        ui->pipelineTable->removeRow(row);
        _pipelineItemList.removeAt(row);
        break;
    default:
        showParameterInformation(row);
        break;
    }


}

// **********************************************************************************+
// BUTTONS

// =========================================================================
// SELECTION STEPS PIPELINE
// =========================================================================
//
void GenericWorkflowDialog::on_cancelSelection_clicked()
{

    enableNextStep(false);
    //enablePipelineArea(false);
    // TODO delete row in table
    // TODO
    restoreUI();
}


void GenericWorkflowDialog::on_stepDoneButton_clicked()
{
    // Adding the info to the pipeline
    insertRow();
    QList<CategoryWkfData> _catList = _wkfData->getCategoryList();
    int _cTab = ui->tabMethods->currentIndex();
    if (_cTab < _catList.size())
    {
        CategoryWkfData _cCategory = _catList.at(_cTab);
        QString _iconPath = _cCategory.getIconPath();
        QString _description = buildDescription(_cCategory,_selectedRow);
        if (_cPipelineRow == -1)
            return;
        _pipelineHelper->updateRow(_iconPath,_description,_cPipelineRow);
        updatePipelineElement(_iconPath, _description, _cCategory,_selectedRow);
    }
    enableNextStep(false);
    saveConfiguration();
    restoreUI();
}


// =========================================================================
// RUN PIPELINE
// =========================================================================

void GenericWorkflowDialog::on_runPipelineButton_clicked()
{
    bool _is3D = true; // At the moment by default!!! Need to be changed in the future!!! TODO

    if (_pipelineItemList.isEmpty())
        return;

    QStringList _parameterList;

    ReaderType3D::Pointer reader = ReaderType3D::New();
    reader->SetFileName(_wkfData->getImagePath().toStdString());
    QString _imageSuffix = QFileInfo(_wkfData->getImagePath()).completeSuffix();

   // Create the rest of the pipeline
    for (int i = 0; i < _pipelineItemList.size();i++)
    {
        PipelineItem  _item = _pipelineItemList.at(i);
        _parameterList = buildParameterList(_item);
        // Creating the pipeline Element
        IOperation * _operation =  OperationFactory::Get()->CreateOperation(_item.getFunctionClassName().toStdString());

        if (i == 0)
            _operation->SetInPut(reader->GetOutput()); // Nota, por algun motivo si meto el reader aqui dentro, pierde la referencia al puntero ¿Smart pointers vida util?? --> Checkear en la conexion del pipeline
        else
        {
            PipelineItem _previousItem = _pipelineItemList.at(i-1);
            if (_is3D)
                _operation->SetInPut(_previousItem.getImage3D());
        }

        _operation->SetParameters(_parameterList);
        _operation->exec();
        QString _imgFileName = TEMP_WKF_PIPELINE_IMAGE_PREFIX + QString::number(i) +"."+ _imageSuffix;
        QString _imgTmpFile = QDir(_imgProjectPath).filePath(_imgFileName);
        _operation->save(_imgTmpFile);
        _item.setImage3DPath(_imgTmpFile);

       _pipelineItemList.replace(i, _item);
    }
    _wkfExecuted = true;
}


QStringList GenericWorkflowDialog::buildParameterList(PipelineItem  _item)
{
    QStringList _parameterList;

    QList<ConfigurationPipelineItem> _configItemList = _item.getConfigurationItemList();

    for (int i = 0; i<  _configItemList.size(); i++)
    {
        ConfigurationPipelineItem _configItem = _configItemList.at(i);

        QString _cValue = _configItem.getCurrentValue();
        if (_cValue.isEmpty())
            _cValue = _configItem.getOptionDefaultValue();
        QString _configValue = _configItem.getMethodName() + PARAMETER_SEPARATOR +_cValue;
        _parameterList.append(_configValue);
    }
    return _parameterList;
}

// =========================================================================
// Accepted BUTTONBOX
// =========================================================================
#include <QMessageBox>



void GenericWorkflowDialog::on_cancelButton_clicked()
{
    _userAcceptChanges = false;
    close();

}

void GenericWorkflowDialog::on_okButton_clicked()
{
    _userAcceptChanges = true;
  if (!_wkfExecuted)
    {
        // TODO launch Message
        QMessageBox::warning(this,QString("Please, run the pipeline!"),QString("Otherwise, your pipeline will be not saved."));
    }
    else
        close();

}
