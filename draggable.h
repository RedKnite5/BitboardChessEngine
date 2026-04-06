
#pragma once
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QPixmap>

#include <functional>

class DraggableLabel : public QLabel {
    Q_OBJECT
public:
    using callbackType = std::function<void(DraggableLabel*, DraggableLabel*, const QPixmap)>;
    explicit DraggableLabel(callbackType aCallback, QWidget *parent = nullptr);

    int row = 0;
    int col = 0;
    callbackType callback;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;
};

