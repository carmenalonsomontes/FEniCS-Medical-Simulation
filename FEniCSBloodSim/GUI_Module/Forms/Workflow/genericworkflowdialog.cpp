#include "genericworkflowdialog.h"
#include "ui_genericworkflowdialog.h"

GenericWorkflowDialog::GenericWorkflowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenericWorkflowDialog)
{
    ui->setupUi(this);
}

GenericWorkflowDialog::~GenericWorkflowDialog()
{
    delete ui;
}
