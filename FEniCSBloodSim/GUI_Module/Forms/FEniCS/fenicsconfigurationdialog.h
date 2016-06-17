#ifndef FENICSCONFIGURATIONDIALOG_H
#define FENICSCONFIGURATIONDIALOG_H

#include <QDialog>
#include <QList>

#include "GUI_Module/Data/FEniCSData/fenicsxmldata.h"
#include "GUI_Module/UIHelpers/workflowtablehelper.h"
#include "GUI_Module/Pipeline/FEniCSPipeline/fenicspipelinedata.h"
#include "GUI_Module/Pipeline/FEniCSPipeline/fenicsparameterpipelinedata.h"

namespace Ui {
class FEniCSConfigurationDialog;
}

class FEniCSConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FEniCSConfigurationDialog(QWidget *parent = 0);
    ~FEniCSConfigurationDialog();    


    bool userAcceptChanges();
private slots:
    void on_dialogButtonBox_accepted();

    void on_dialogButtonBox_rejected();

    void on_stepDoneButton_clicked();

    void on_cancelConfigButton_clicked();

    void on_firstTabTable_cellClicked(int row, int column);

    void on_fenicsPipelineTable_cellClicked(int row, int column);

    void on_fenicsPipelineOptionsTable_cellChanged(int row, int column);

private:
    Ui::FEniCSConfigurationDialog *ui;

    FEniCSXMLData * _fenicsData;
    WorkflowTableHelper * _fenicsTableTabeHelper;
    WorkflowTableHelper * _fenicsOptionsTableHelper;
    WorkflowTableHelper * _fenicsPipelineTableHelper;
    WorkflowTableHelper * _fenicsPipelineOptionsTableHelper;
    int _cPipelineRow;
    int _selectedRow;

    QList<FEniCSPipelineData> _pipelineData;


    bool _userAcceptChanges;

    void loadFenicsData();
    void initializeHelpers();

    void createCategoryTabs();
    void createTab(CategoryFEniCSData category, int tabIndex);
    void createTabTables(int noTable);
    void fillTableWithInformation(int index);
    void fillMenuTable(CategoryFEniCSData _catObj);
    void insertConfigurationOptions(int row,int column);
    void addParametersToConfigurationTable(int row,CategoryFEniCSData cat);


    void insertRow();
    QString buildDescription(CategoryFEniCSData _cCategory, int _row);
    void enableNextStep(bool _val);
    void restoreUI();

    void showParameterInformation(int row);
    void moveDown(int row);
    void moveUp(int row);
    void updatePipelineElement(QString _description, CategoryFEniCSData _category,int noFunction);
};

#endif // FENICSCONFIGURATIONDIALOG_H
