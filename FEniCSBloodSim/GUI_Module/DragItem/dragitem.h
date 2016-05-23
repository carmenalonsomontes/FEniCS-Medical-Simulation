#ifndef DRAGITEM_H
#define DRAGITEM_H
#include <QFrame>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>


class DragItem: public QFrame
{
public:
    DragItem(QWidget *parent = 0);
    void insertItem( QString _iconPath, QString _description);
protected:
    void dragEnterEvent(QDragEnterEvent *event);// Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event); // Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event); // Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event);// Q_DECL_OVERRIDE;
};

#endif // DRAGITEM_H
