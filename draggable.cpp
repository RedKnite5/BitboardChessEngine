#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

#include <functional>

#include "draggable.h"

DraggableLabel::DraggableLabel(
    callbackType aCallback,
    QWidget *parent
)
    : QLabel(parent)
{
    callback = aCallback;
    setAcceptDrops(true);
}

void DraggableLabel::mousePressEvent(QMouseEvent *event) {
    if (pixmap(Qt::ReturnByValue).isNull() || event->button() != Qt::LeftButton)
        return;

    QMimeData *mimeData = new QMimeData;
    mimeData->setImageData(pixmap(Qt::ReturnByValue).toImage());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap(Qt::ReturnByValue));
    drag->setHotSpot(QPoint(drag->pixmap().width() / 2, drag->pixmap().height() / 2));
    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void DraggableLabel::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasImage())
        event->acceptProposedAction();
}

void DraggableLabel::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasImage()) {


        const QPixmap *oldPixmapPtr = pixmap();
        QPixmap oldPixmap;
        if (oldPixmapPtr) {
            oldPixmap = *pixmap();
        } else {
            oldPixmap = QPixmap();
        }

        QPixmap droppedPixmap = QPixmap::fromImage(qvariant_cast<QImage>(event->mimeData()->imageData()));
        setPixmap(droppedPixmap);

        //QPixmap droppedPixmap = QPixmap::fromImage(qvariant_cast<QImage>(event->mimeData()->imageData()));
        //setPixmap(droppedPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        DraggableLabel *source = qobject_cast<DraggableLabel*>(event->source());
        if (source && source != this) {
            source->clear();
        }

        event->acceptProposedAction();

        callback(source, this, oldPixmap);
    }
}
