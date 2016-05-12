#ifndef GENERICWORKFLOWDIALOG_H
#define GENERICWORKFLOWDIALOG_H

#include <QDialog>

#include "GUI_Module/UIHelpers/workflowtablehelper.h"
#include "GUI_Module/DragItem/dragitem.h"

namespace Ui {
class GenericWorkflowDialog;
}

class GenericWorkflowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GenericWorkflowDialog(QWidget *parent = 0);
    ~GenericWorkflowDialog();

protected:
    WorkflowTableHelper * _wkfHelper;
    DragItem * _dragableArea ;

    void createTabWithName(int tabIndex, const QString text);

private slots:

    void on_wkfButtonBox_accepted();
    void on_tableMethods1_cellClicked(int row, int column);
    void on_tabMethods_currentChanged(int index);

private:
    Ui::GenericWorkflowDialog *ui;
    void loadIcosn(int row);

};

#endif // GENERICWORKFLOWDIALOG_H
