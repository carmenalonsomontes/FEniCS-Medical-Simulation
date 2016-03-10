#ifndef RENDERWINDOWUISINGLEINHERITANCE_H
#define RENDERWINDOWUISINGLEINHERITANCE_H

#include <QMainWindow>

namespace Ui {
class RenderWindowUISingleInheritance;
}

class RenderWindowUISingleInheritance : public QMainWindow
{
    Q_OBJECT

public:
    explicit RenderWindowUISingleInheritance(QWidget *parent = 0);
    ~RenderWindowUISingleInheritance();


//public slots:

  //virtual void slotExit();

private:
    Ui::RenderWindowUISingleInheritance *ui;
};

#endif // RENDERWINDOWUISINGLEINHERITANCE_H
