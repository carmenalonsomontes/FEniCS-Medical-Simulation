#include "dragablewidget.h"

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QPalette>
#include <QPoint>
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QStringList>
#include <QDrag>

#include "draglabel.h"

DragableWidget::DragableWidget(QWidget *parent)
    : QWidget(parent)
{
    QFile dictionaryFile(":/dictionary/words.txt");
    dictionaryFile.open(QFile::ReadOnly);
    QTextStream inputStream(&dictionaryFile);

    int x = 5;
    int y = 5;

    while (!inputStream.atEnd()) {
        QString word;
        inputStream >> word;
        if (!word.isEmpty()) {
            DragLabel *wordLabel = new DragLabel(word, this);
            wordLabel->move(x, y);
            wordLabel->show();
            x += wordLabel->width() + 2;
            if (x >= 245) {
                x = 5;
                y += wordLabel->height() + 2;
            }
        }
    }

    QPalette newPalette = palette();
    newPalette.setColor(QPalette::Window, Qt::white);
    setPalette(newPalette);

    setMinimumSize(400, qMax(200, y));
    setWindowTitle(tr("Fridge Magnets"));
    setAcceptDrops(true);
}

void DragableWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void DragableWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void DragableWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
        const QMimeData *mime = event->mimeData();
        QByteArray itemData = mime->data("application/x-fridgemagnet");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString text;
        QPoint offset;
        dataStream >> text >> offset;

        DragLabel *newLabel = new DragLabel(text, this);
        newLabel->move(event->pos() - offset);
        newLabel->show();

        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        QStringList pieces = event->mimeData()->text().split(QRegExp("\\s+"),
                             QString::SkipEmptyParts);
        QPoint position = event->pos();

        foreach (QString piece, pieces) {
            DragLabel *newLabel = new DragLabel(piece, this);
            newLabel->move(position);
            newLabel->show();

            position += QPoint(newLabel->width(), 0);
        }

        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}
