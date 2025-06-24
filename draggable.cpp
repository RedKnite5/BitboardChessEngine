#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

#include "draggable.h"

DraggableLabel::DraggableLabel(QWidget *parent)
    : QLabel(parent)
{
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
    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void DraggableLabel::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasImage())
        event->acceptProposedAction();
}

void DraggableLabel::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasImage()) {
        QPixmap droppedPixmap = QPixmap::fromImage(qvariant_cast<QImage>(event->mimeData()->imageData()));
        setPixmap(droppedPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        event->acceptProposedAction();
    }
}
