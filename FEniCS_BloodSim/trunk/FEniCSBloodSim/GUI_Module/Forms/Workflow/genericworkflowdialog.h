#ifndef GENERICWORKFLOWDIALOG_H
#define GENERICWORKFLOWDIALOG_H

#include <QDialog>

namespace Ui {
class GenericWorkflowDialog;
}

class GenericWorkflowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GenericWorkflowDialog(QWidget *parent = 0);
    ~GenericWorkflowDialog();

private:
    Ui::GenericWorkflowDialog *ui;
};

#endif // GENERICWORKFLOWDIALOG_H
