#ifndef GENERICWORKFLOWDIALOG_H
#define GENERICWORKFLOWDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QList>

#include "GUI_Module/UIHelpers/workflowtablehelper.h"
//#include "GUI_Module/DragItem/dragitem.h"
#include "GUI_Module/Data/WorkflowData/workflowdata.h"
#include "GUI_Module/Data/WorkflowData/categorywkfdata.h"
#include "GUI_Module/Data/WorkflowData/imagingwkffunctiondata.h"

#include "GUI_Module/Forms/Workflow/Pipeline/pipelineitem.h"

namespace Ui {
class GenericWorkflowDialog;
}

class GenericWorkflowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GenericWorkflowDialog(QWidget *parent = 0);
    ~GenericWorkflowDialog();
    QList<PipelineItem> getPipelineItemList();
    bool userAcceptChanges();

protected:
    WorkflowTableHelper * _wkfHelper;
    WorkflowTableHelper * _pipelineHelper;
    WorkflowTableHelper * _configurationHelper;
    WorkflowTableHelper * _summaryHelper;
   // DragItem * _dragableArea ;
    WorkflowData  _wkfData;

    void createTabWithName(int tabIndex, const QString text);



private slots:

    void on_wkfButtonBox_accepted();
    void on_tableMethods1_cellClicked(int row, int column);
    void on_tabMethods_currentChanged(int index);
    //void on_addStepToPipelineButton_clicked();

    void on_stepDoneButton_clicked();

    void on_pipelineTable_cellClicked(int row, int column);


    void on_runPipelineButton_clicked();

private:
    Ui::GenericWorkflowDialog *ui;
    int _cPipelineRow;
    QList<PipelineItem> _pipelineItemList;
    bool _userAcceptChanges;


    void fillTableWithInformation(int index);
    void insertRow();
    void enableNextStep(bool _val);
    void updateValuesPipelineTable(int row,int column);
    QString buildDescription(CategoryWkfData _cCategory, int _row);
    void restoreUI();
    void addSignalPipelineRow();
    void updatePipelineElement(QString _iconPath, QString _description, QString _categoryName);

    void moveDown(int row);
    void moveUp(int row);
    void addParametersToConfigurationTable(int row,CategoryWkfData cat);
    void showParameterInformation(int row);
    void saveConfiguration();
    void createTabTables(int noTable);
};

#endif // GENERICWORKFLOWDIALOG_H
