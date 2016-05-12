#ifndef DRAGABLEWIDGET_H
#define DRAGABLEWIDGET_H

#include <QFrame>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>


class DragableWidget : public QFrame
{
public:
    DragableWidget(QWidget *parent = 0);
protected:
    void dragEnterEvent(QDragEnterEvent *event);// Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event); // Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event); // Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event);// Q_DECL_OVERRIDE;
};

#endif // DRAGABLEWIDGET_H
