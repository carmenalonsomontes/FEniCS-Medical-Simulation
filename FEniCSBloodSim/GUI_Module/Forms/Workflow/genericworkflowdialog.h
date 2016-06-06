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
    WorkflowData  * _wkfData;

    void createTabWithName(int tabIndex, const QString text);
    void createTabTables(int noTable);
    void fillTableWithInformation(int index);

private slots:

    void on_wkfButtonBox_accepted();
    void on_tableMethods1_cellClicked(int row, int column);
    void on_stepDoneButton_clicked();
    void on_pipelineTable_cellClicked(int row, int column);
    void on_runPipelineButton_clicked();

    void on_cancelSelection_clicked();

private:
    Ui::GenericWorkflowDialog *ui;
    int _cPipelineRow;
    QList<PipelineItem> _pipelineItemList;
    bool _userAcceptChanges;

    int _selectedRow;



    void insertRow();

    void updateValuesPipelineTable(int row,int column);
    QString buildDescription(CategoryWkfData _cCategory, int _row);

    void addSignalPipelineRow();
    void updatePipelineElement(QString _iconPath, QString _description, CategoryWkfData _category,int noFunction);

    void moveDown(int row);
    void moveUp(int row);
    void addParametersToConfigurationTable(int row,CategoryWkfData cat);
    void showParameterInformation(int row);
    void saveConfiguration();



    QStringList buildParameterList(PipelineItem _item);
    void runPipelineItem( QStringList _parameterList,QString className);

    void enableNextStep(bool _val);
    //void enablePipelineArea(bool _val);
    void restoreUI();
};

#endif // GENERICWORKFLOWDIALOG_H
