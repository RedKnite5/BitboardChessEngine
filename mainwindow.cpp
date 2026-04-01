#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>

#include "draggable.h"

template <typename T>
using Array8x8 = std::array<std::array<T, 8>, 8>;

enum Piece {
    BlackPawn,    // 0
    BlackKnight,  // 1
    BlackBishop,  // 2
    BlackRook,    // 3
    BlackQueen,   // 4
    BlackKing,    // 5
    WhitePawn,    // 6
    WhiteKnight,  // 7
    WhiteBishop,  // 8
    WhiteRook,    // 9
    WhiteQueen,   // 10
    WhiteKing     // 11
};

constexpr std::array<std::string_view, 12> PieceImages = {
    "BlackPawn",
    "BlackKnight",
    "BlackBishop",
    "BlackRook",
    "BlackQueen",
    "BlackKing",
    "WhitePawn",
    "WhiteKnight",
    "WhiteBishop",
    "WhiteRook",
    "WhiteQueen",
    "WhiteKing"
};


void setPieceImage(const char *piece, DraggableLabel *square) {
    QPixmap pixmap(QString(":/pieces/") + piece + ".png");
    square->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void updatePieceImages(const Array8x8<int> &board, Array8x8<DraggableLabel *> &squares) {
    for (int row=0; row<8; ++row) {
        for (int col=0; col<8; ++col) {
            int piece = board[row][col];
            if (piece != -1) {
                setPieceImage(PieceImages[piece].data(), squares[row][col]);
            } else {
                squares[row][col]->clear();
            }
        }
    }
}

void setupChessboard(QWidget *parent, Array8x8<int> &board) {
    QGridLayout *layout = new QGridLayout(parent);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    Array8x8<DraggableLabel*> squares;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            DraggableLabel *square = new DraggableLabel(parent);
            square->setFixedSize(60, 60);
            square->setAlignment(Qt::AlignCenter);

            QColor color = ((row + col) % 2 == 1) ? Qt::white : Qt::gray;
            QPalette palette = square->palette();
            palette.setColor(QPalette::Window, color);
            square->setAutoFillBackground(true);
            square->setPalette(palette);

            square->setObjectName(QString("square_%1").arg(row * 8 + col + 1));
            squares[row][col] = square;
            square->row = row;
            square->col = col;

            layout->addWidget(square, 7-row, col);
        }
    }

    board[0][0] = WhiteRook;
    board[0][1] = WhiteBishop;
    board[0][2] = WhiteKnight;
    board[0][3] = WhiteQueen;
    board[0][4] = WhiteKing;
    board[0][5] = WhiteKnight;
    board[0][6] = WhiteBishop;
    board[0][7] = WhiteRook;

    for (int i=0; i<8; ++i) {
        board[1][i] = WhitePawn;
        board[6][i] = BlackPawn;

        board[7][i] = board[0][i] - 6;  // reflect black pieces
    }

    updatePieceImages(board, squares);
    
    parent->setLayout(layout);
}




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Array8x8<int> board;
    for (auto& row : board) {
        row.fill(-1);
    }


    QWidget *gridLayout = findChild<QWidget *>("gridLayoutWidget");
    gridLayout->setFixedSize(480, 480);
    if (gridLayout) {
        setupChessboard(gridLayout, board);
    } else {
        qWarning("Grid layout widget not found in the UI.");
    }
    


    
}

MainWindow::~MainWindow()
{
    delete ui;
}

