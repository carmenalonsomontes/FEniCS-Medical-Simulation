#include "fenics_blood_sim.h"
#include "ui_fenics_blood_sim.h"

FEniCS_Blood_Sim::FEniCS_Blood_Sim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FEniCS_Blood_Sim)
{
    ui->setupUi(this);
}

FEniCS_Blood_Sim::~FEniCS_Blood_Sim()
{
    delete ui;
}
