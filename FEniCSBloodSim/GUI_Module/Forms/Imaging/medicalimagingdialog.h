#ifndef MEDICALIMAGINGDIALOG_H
#define MEDICALIMAGINGDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class MedicalImagingDialog;
}

class MedicalImagingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedicalImagingDialog(QWidget *parent = 0);
    ~MedicalImagingDialog();

    QString getDataSetPath();
    QString getSeriesImagePrefix();
    QString getImagePath();
    QString getImageSuffix();

    void setDataSetPath(QString pathfile);
    void setSeriesImagePrefix(QString pathfile);
    void setImagePath(QString pathfile);
    void setImageSuffix(QString imsuffix);

private slots:


    void on_imPathOption1ToolButton_clicked();

    void on_startImagetoolButton_clicked();

    void on_acceptRejectButton_accepted();

    void on_acceptRejectButton_rejected();

    void on_helpButton_clicked();

private:
    Ui::MedicalImagingDialog *ui;
    QString _dataSetPath;
    QString _imPath;
    QString _prefixIm;
    QString _suffixIm;

    QString  selectFile();
};

#endif // MEDICALIMAGINGDIALOG_H
