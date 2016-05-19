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
   //QString text2 = "<img src=\":/wkf/images/GUI_Module/images/images/color_adjustment.png\"/> Terminal settings";
     QString text2="Mi ejemplo \n vaya vaya";

     QTextDocument doc;
     doc.setHtml(text);// "<p>A QTextDocument can be used to present formatted text  in a nice way.</p> <p align=center>It can be <b>formatted</b> "
                  //"<font size=+2>in</font> <i>different</i> ways.</p>"
                  //"<p>The text can be really long and contain many "
                  //"paragraphs. It is properly wrapped and such...</p>" );


     QFontMetrics metric(font());
     QSize size = doc.size().toSize();//metric.size(Qt::TextSingleLine | Qt::TextWordWrap,text);

    QImage image(size.width() + 10, size.height() + 10,
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
    //--------------------


     QRect rect = QRect(QPoint(0, 0), size);


     //rect.translate( 0, rect.height()+10 );
     //rect.setHeight( 300 );
     doc.setTextWidth( rect.width() );
     painter.translate( rect.topLeft() );
     doc.drawContents( &painter, rect.translated( -rect.topLeft() ) );

     //-----------------------------------------


 /*  //  painter.drawText(QRect(QPoint(6, 6), size),Qt::TextWordWrap, text);
      painter.drawText(QRect(QPoint(0, 0), size),Qt::AlignTop | Qt::AlignCenter | Qt::TextWordWrap, text);
    //  painter.drawText(QRect(QPoint(6, 6), size),Qt::AlignTop | Qt::AlignCenter | Qt::TextWordWrap, text2);//tr("Qt by\nNokia"));
   //  QPainter::drawText(QRect(QPoint(6, 6), size), Qt::TextWordWrap,text2);
     */
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
