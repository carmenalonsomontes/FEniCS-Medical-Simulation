#include "fenics_bs_gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FEniCS_BS_GUI w;
    w.show();

    return a.exec();
}
