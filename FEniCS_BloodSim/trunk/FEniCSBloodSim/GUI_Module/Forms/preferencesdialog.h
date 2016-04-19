#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

    void setMeshPath(QString meshPath);
    QString getMeshPath();

    void setFENICSPath(QString fenicsPath);
    QString getFENICSPath();

    bool userAcceptChanges();


private slots:
    void on_meshToolButton_clicked();

    void on_fenicsToolButton_clicked();

    void on_preferenceButtonBox_accepted();

private:
    Ui::PreferencesDialog *ui;

    QString _meshPath;
    QString _fenicsPath;

    bool _userAcceptchanges;

    QString openSelectDialog();

   // void clearInterface();

    void updateUI();

};

#endif // PREFERENCESDIALOG_H
