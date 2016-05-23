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

#include <QTextDocument>

DragLabel::DragLabel(const QString &text, QWidget *parent)
     : QLabel(parent)
 {
     QTextDocument doc;
     doc.setHtml(text);


     QFontMetrics metric(font());
     QSize size = doc.size().toSize();

    QImage image(size.width() + 20, size.height() + 20,
                  QImage::Format_ARGB32_Premultiplied);
     image.fill(qRgba(0, 0, 0, 0));

     QFont font;
     font.setStyleStrategy(QFont::ForceOutline);


     QPainter painter;
     painter.begin(&image);
     painter.setRenderHint(QPainter::Antialiasing);
     painter.setBrush(Qt::gray);
     painter.drawRect(QRectF(0.5, 0.5, image.width()-1, image.height()-1));

     painter.setFont(font);
     painter.setBrush(Qt::black);

     QRect rect = QRect(QPoint(0, 0), size);
     rect.setHeight( 300 );
     doc.setTextWidth( rect.width() );
     painter.translate( rect.topLeft() );
     doc.drawContents( &painter, rect.translated( -rect.topLeft() ) );

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
