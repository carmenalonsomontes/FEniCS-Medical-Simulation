#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include <QCoreApplication>
#include <QFileDialog>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    _userAcceptchanges = false;

}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::on_meshToolButton_clicked()
{
    _meshPath = openSelectDialog();
    updateUI();
}

void PreferencesDialog::on_fenicsToolButton_clicked()
{
   _fenicsPath =  openSelectDialog();
   updateUI();
}

void PreferencesDialog::on_preferenceButtonBox_accepted()
{
    _userAcceptchanges = true;
    close();
}


QString PreferencesDialog::openSelectDialog()
{
    QString _projectPath = QFileDialog::getOpenFileName(this, "Select the binary of the tool",QCoreApplication::applicationDirPath(),
                                                       "All (*)");
   return _projectPath;
}

void PreferencesDialog::updateUI()
{
    if (!_meshPath.isEmpty())
        ui->meshLineEdit->setText(_meshPath);
    if (!_fenicsPath.isEmpty())
        ui->fenicsLineEdit->setText(_fenicsPath);
}

/*
void PreferencesDialog::clearInterface()
{
    _meshPath = "";
    _fenicsPath = "";
    ui->fenicsLineEdit->clear();
    ui->meshLineEdit->clear();
}*/


void PreferencesDialog::setMeshPath(QString meshPath)
{
    _meshPath = meshPath;
    ui->meshLineEdit->setText(_meshPath);
}

QString PreferencesDialog::getMeshPath()
{
    return _meshPath;
}

void PreferencesDialog::setFENICSPath(QString fenicsPath)
{
    _fenicsPath = fenicsPath;
    ui->fenicsLineEdit->setText(_fenicsPath);
}

QString PreferencesDialog::getFENICSPath()
{
    return _fenicsPath;

}
bool PreferencesDialog::userAcceptChanges()
{
    return _userAcceptchanges;
}
