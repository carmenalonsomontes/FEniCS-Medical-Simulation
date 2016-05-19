#ifndef DRAGABLEWIDGET_H
#define DRAGABLEWIDGET_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>


class DragableWidget : public QWidget
{
public:
    DragableWidget(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // DRAGABLEWIDGET_H
