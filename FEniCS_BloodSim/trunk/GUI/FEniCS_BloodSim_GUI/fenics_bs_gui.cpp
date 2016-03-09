#include "fenics_bs_gui.h"
#include "ui_fenics_bs_gui.h"
//#include <QVTKWidget.h>
#include <QGridLayout>

FEniCS_BS_GUI::FEniCS_BS_GUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FEniCS_BS_GUI)
{
    ui->setupUi(this);

    // QVTKWidget * qvtkwidget = new QVTKWidget();
     QGridLayout *layout = new QGridLayout;
     //layout->addWidget(qvtkwidget);
}

FEniCS_BS_GUI::~FEniCS_BS_GUI()
{
    delete ui;
}
