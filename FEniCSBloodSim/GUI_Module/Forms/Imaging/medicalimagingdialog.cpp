#include "medicalimagingdialog.h"
#include "ui_medicalimagingdialog.h"

MedicalImagingDialog::MedicalImagingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MedicalImagingDialog)
{
    ui->setupUi(this);
}

MedicalImagingDialog::~MedicalImagingDialog()
{
    delete ui;
}
