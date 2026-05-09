
#pragma once
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QPixmap>

#include <functional>

class DragDropState {
public:
    bool enabled = true;
};


class DraggableLabel;

class GuiMove {
public:
    DraggableLabel *source;
    DraggableLabel *dest;
    QPixmap taken_piece;
};

class DraggableLabel : public QLabel {
    Q_OBJECT
public:
    using callbackType = std::function<void(GuiMove)>;
    explicit DraggableLabel(
        callbackType aCallback,
        std::shared_ptr<DragDropState> state,
        QWidget *parent = nullptr);

    int row = 0;
    int col = 0;
    callbackType callback;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

private:
    std::shared_ptr<DragDropState> m_state;
};



