#ifndef FENICS_BS_GUI_H
#define FENICS_BS_GUI_H

#include <QMainWindow>

namespace Ui {
class FEniCS_BS_GUI;
}

class FEniCS_BS_GUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit FEniCS_BS_GUI(QWidget *parent = 0);
    ~FEniCS_BS_GUI();

private:
    Ui::FEniCS_BS_GUI *ui;
};

#endif // FENICS_BS_GUI_H
