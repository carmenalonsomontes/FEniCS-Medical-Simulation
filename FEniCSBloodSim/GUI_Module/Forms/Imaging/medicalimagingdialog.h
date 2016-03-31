#ifndef MEDICALIMAGINGDIALOG_H
#define MEDICALIMAGINGDIALOG_H

#include <QDialog>

namespace Ui {
class MedicalImagingDialog;
}

class MedicalImagingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedicalImagingDialog(QWidget *parent = 0);
    ~MedicalImagingDialog();

private:
    Ui::MedicalImagingDialog *ui;
};

#endif // MEDICALIMAGINGDIALOG_H
