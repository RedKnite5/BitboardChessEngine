#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>
#include <vector>
#include <time.h>

#include "draggable.h"

#include "search.h"
#include "board.h"

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


void MainWindow::engineTurn() {
    const int depth = 8;

    long long start = current_time_us();
    int move = game.S.negamax(game.board, depth);
    //move = S.pv[0];
    long long duration = current_time_us() - start;

    print_move(move);

    printf("\n\nTime: %lld ms\n", duration / 1000);
    printf("Time: %lld s\n", duration / 1000000);

    make_move(game.board, move);


    int source = get_move_source(move);
    int dest = get_move_target(move);

    DraggableLabel *sourceSquare = squares[source / 8][source % 8];
    DraggableLabel *destSquare = squares[dest / 8][dest % 8];

    
    const QPixmap *pieceImage = sourceSquare->pixmap();
    if (pieceImage && !pieceImage->isNull()) {
        destSquare->setPixmap(*pieceImage);
        sourceSquare->setPixmap(QPixmap()); 
    } else {
        printf("source: %d is empty\n", source);
    }
}



int findUserMove(int userSource, int userDest, const std::vector<int> &move_list) {
    for (auto move : move_list) {
        int source_sq = get_move_source(move);
        int target_sq = get_move_target(move);

        if (source_sq == userSource && userDest == target_sq) {
            return move;
        }
    }
    return 0;
}


void MainWindow::playerMoved(DraggableLabel *sourceSquare, DraggableLabel *destSquare, const QPixmap takenImage) {
    printf("Callback\n");

    std::vector<int> move_list;

    generate_moves(game.board, move_list);

    int userSource = sourceSquare->row * 8 + sourceSquare->col;
    int userDest = destSquare->row * 8 + destSquare->col;
    int userMove = findUserMove(userSource, userDest, move_list);

    

    if (userMove) {
        // user move is psuedolegal!

        Board new_board = game.board;
        bool king_safe = make_move(new_board, userMove);
        if (king_safe) {
            // user move is legal!

            game.board = new_board;

            engineTurn();
            return;
        }
    }

    const QPixmap *pieceImage = destSquare->pixmap();
    if (pieceImage && !pieceImage->isNull()) {
        sourceSquare->setPixmap(*pieceImage);
        destSquare->setPixmap(takenImage); 
    } else {
        printf(
            "While trying to undo illegal move: source %d is empty\n",
            userSource
        );
    }

    printf("User made illegal move from row %d col %d to row %d col %d\n",
        sourceSquare->row, sourceSquare->col,
        destSquare->row, destSquare->col);
}



Array8x8<DraggableLabel*> setupChessboard(MainWindow *mainwindow, QWidget *parent) {
    QGridLayout *layout = new QGridLayout(parent);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    Array8x8<DraggableLabel*> squares;

    auto boundMethod = [mainwindow](DraggableLabel *src, DraggableLabel *dst, const QPixmap takenImage) {
        mainwindow->playerMoved(src, dst, takenImage);
    };

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            DraggableLabel *square = new DraggableLabel(
                boundMethod,
                parent);
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
    
    parent->setLayout(layout);
    return squares;
}

void setupPieces(Array8x8<int> &board, Board &bitboard) {
    for (int i=0; i<64; ++i) {
        if ((bitboard.allPieces & (1ULL << i)) == 0) {
            continue;
        }
        for (int p=0; p<12; ++p) {
            if (bitboard.bitboards[p] & (1ULL << i)) {
                board[i / 8][i % 8] = p;
                continue;
            }

        }
    }
}


MainWindow::MainWindow(Game &aGame, QWidget *parent)
    : QMainWindow(parent),
    game(aGame),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Array8x8<int> board;
    for (auto& row : board) {
        row.fill(-1);
    }


    QWidget *gridLayout = findChild<QWidget *>("gridLayoutWidget");
    gridLayout->setFixedSize(480, 480);
    if (gridLayout) {
        squares = setupChessboard(this, gridLayout);
    } else {
        qWarning("Grid layout widget not found in the UI.");
    }

    setupPieces(board, game.board);
    updatePieceImages(board, squares);
    

    
}

MainWindow::~MainWindow()
{
    delete ui;
}



