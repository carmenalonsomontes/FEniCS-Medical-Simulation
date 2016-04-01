#include "medicalimagingdialog.h"
#include "ui_medicalimagingdialog.h"

#include <QFileDialog>
#include <QCoreApplication>
#include <QFileInfo>

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


QString MedicalImagingDialog::getDataSetPath()
{
    return _dataSetPath;
}

QString MedicalImagingDialog::getSeriesImagePrefix()
{
    return _prefixIm;
}

QString MedicalImagingDialog::getImagePath()
{
    return _imPath;
}

QString MedicalImagingDialog::getImageSuffix()
{
    return _suffixIm;
}

void MedicalImagingDialog::setDataSetPath(QString pathfile)
{
    _dataSetPath = pathfile;
     ui->datasetLineEdit->setText(_dataSetPath);
}

void MedicalImagingDialog::setSeriesImagePrefix(QString pathfile)
{
    _prefixIm = pathfile;
    ui->imPrefixLineEdit->setText(_prefixIm);

}

void MedicalImagingDialog::setImageSuffix(QString imsuffix)
{
    _suffixIm = imsuffix;
}

void MedicalImagingDialog::setImagePath(QString pathfile)
{
    _imPath = pathfile;
    ui->imPathOption1LineEdit->setText(_imPath);
}

//***********************************************************************
/** Note: only one option is allowed, or you choose an image or you choose a dataseries
 * */
void MedicalImagingDialog::on_imPathOption1ToolButton_clicked()
{

    _imPath = selectFile();
    ui->imPathOption1LineEdit->setText(_imPath);

    // Removing the other values, since this option was chosen by the user
    _prefixIm.clear();
    _suffixIm.clear();
    _dataSetPath.clear();



}

void MedicalImagingDialog::on_startImagetoolButton_clicked()
{
    QString _selectedPath = selectFile();
    if ((_selectedPath.isEmpty()) || (_selectedPath.isNull()))
        return;

    QString dataSetPath = QFileInfo(_selectedPath).absolutePath();
    setDataSetPath(dataSetPath);

    QString imPrefix= QFileInfo(_selectedPath).baseName();
    setSeriesImagePrefix(imPrefix);

    _suffixIm =  QFileInfo(_selectedPath).completeSuffix();

    // Removing _imPath, since this option was chosen by the user
    _imPath.clear();

}




//****************************************************************************
void MedicalImagingDialog::on_acceptRejectButton_accepted()
{
    close();
}

void MedicalImagingDialog::on_acceptRejectButton_rejected()
{
    _imPath.clear();
    _prefixIm.clear();
    _dataSetPath.clear();
    close();
}

void MedicalImagingDialog::on_helpButton_clicked()
{
 //TODO
}


QString  MedicalImagingDialog::selectFile()
{
    QString projectPath = QFileDialog::getOpenFileName(this, "Select the image you want to process",QCoreApplication::applicationDirPath(),
                                                       "MRA Images (*.nii.gz *.mha)");
    return projectPath;
}
