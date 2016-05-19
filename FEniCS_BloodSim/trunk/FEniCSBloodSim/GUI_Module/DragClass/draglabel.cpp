#include "draglabel.h"

#include <QFontMetrics>
#include <QSize>
#include <QImage>
#include <QFont>
#include <QLinearGradient>
#include <QPainter>
#include <QMimeData>
#include <QDrag>
#include <QDataStream>

#include <QMouseEvent>

DragLabel::DragLabel(const QString &text, QWidget *parent)
     : QLabel(parent)
 {
     QFontMetrics metric(font());
     QSize size = metric.size(Qt::TextSingleLine, text);

     QImage image(size.width() + 12, size.height() + 12,
                  QImage::Format_ARGB32_Premultiplied);
     image.fill(qRgba(0, 0, 0, 0));

     QFont font;
     font.setStyleStrategy(QFont::ForceOutline);

     QPainter painter;
     painter.begin(&image);
     painter.setRenderHint(QPainter::Antialiasing);
     painter.setBrush(Qt::white);
     painter.drawRoundRect(QRectF(0.5, 0.5, image.width()-1, image.height()-1),
                           25, 25);

     painter.setFont(font);
     painter.setBrush(Qt::black);
     painter.drawText(QRect(QPoint(6, 6), size), Qt::AlignCenter, text);
     painter.end();

     setPixmap(QPixmap::fromImage(image));
     labelText = text;
 }



 void DragLabel::mousePressEvent(QMouseEvent *event)
 {
     QByteArray itemData;
     QDataStream dataStream(&itemData, QIODevice::WriteOnly);
     dataStream << labelText << QPoint(event->pos() - rect().topLeft());

     QMimeData *mimeData = new QMimeData;
     mimeData->setData("application/x-fridgemagnet", itemData);
     mimeData->setText(labelText);

     QDrag *drag = new QDrag(this);
     drag->setMimeData(mimeData);
     drag->setHotSpot(event->pos() - rect().topLeft());
     drag->setPixmap(*pixmap());

     hide();

     if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
         close();
     else
         show();
 }
