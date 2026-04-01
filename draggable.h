
#pragma once
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

class DraggableLabel : public QLabel {
    Q_OBJECT
public:
    explicit DraggableLabel(QWidget *parent = nullptr);

    int row = 0;
    int col = 0;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;
};

