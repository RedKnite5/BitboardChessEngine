#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>

#include "draggable.h"


void setupChessboard(QWidget *parent) {
    QGridLayout *layout = new QGridLayout(parent);
    layout->setSpacing(0);  // No spacing between squares

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            DraggableLabel *square = new DraggableLabel(parent);
            square->setFixedSize(60, 60);  // Adjust size as needed
            square->setAlignment(Qt::AlignCenter);

            // Alternate color: white and gray
            QColor color = ((row + col) % 2 == 0) ? Qt::white : Qt::gray;
            QPalette palette = square->palette();
            palette.setColor(QPalette::Window, color);
            square->setAutoFillBackground(true);
            square->setPalette(palette);

            // Optional: Name the square (e.g., square_1 to square_64)
            square->setObjectName(QString("square_%1").arg(row * 8 + col + 1));

            layout->addWidget(square, row, col);
        }
    }

    parent->setLayout(layout);
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QWidget *gridLayout = findChild<QWidget *>("gridLayoutWidget");
    if (gridLayout) {
        setupChessboard(gridLayout);
    } else {
        qWarning("Grid layout widget not found in the UI.");
    }
    


    
}

MainWindow::~MainWindow()
{
    delete ui;
}

