#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QTimer>
#include <QApplication>

#include <functional>

#include "draggable.h"

DraggableLabel::DraggableLabel(
        callbackType aCallback,
        std::shared_ptr<DragDropState> state,
        QWidget *parent)
    : QLabel(parent)
{
    callback = aCallback;
    m_state = state;
    setAcceptDrops(true);
}

void DraggableLabel::mousePressEvent(QMouseEvent *event) {
    if (!m_state->enabled) return;

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
    if (!m_state->enabled) return;

    if (event->mimeData()->hasImage()) {
        event->acceptProposedAction();
    }
}

void DraggableLabel::dropEvent(QDropEvent *event) {
    if (!m_state->enabled) return;

    if (event->mimeData()->hasImage()) {

        QPixmap oldPixmap = pixmap(Qt::ReturnByValue);

        QPixmap droppedPixmap = QPixmap::fromImage(qvariant_cast<QImage>(event->mimeData()->imageData()));
        setPixmap(droppedPixmap);

        DraggableLabel *source = qobject_cast<DraggableLabel*>(event->source());
        if (source && source != this) {
            source->clear();
        }

        event->acceptProposedAction();
        //QApplication::processEvents();

        GuiMove move{source, this, oldPixmap};

        callback(move);
    }
}
