#ifndef DRAGLABEL_H
#define DRAGLABEL_H
//class QDragEnterEvent;
//class QDragMoveEvent;
//class QFrame;

#include <QLabel>
#include <QString>


class DragLabel : public QLabel
{
public:
     DragLabel(const QString &text, QWidget *parent);

 protected:
     void mousePressEvent(QMouseEvent *event);

 private:
     QString labelText;
};

#endif // DRAGLABEL_H


