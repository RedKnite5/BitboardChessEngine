#include <vector>
#include <time.h>

#include <QGridLayout>
#include <QDialog>
#include <QThread>
#include <QMetaType>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "draggable.h"
#include "search.h"
#include "board.h"
#include "engine_worker.h"

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

const int pieceSize = 50;

void setPieceImage(const int piece_int, QLabel *square) {
    const char *piece = PieceImages[piece_int].data();
    QPixmap pixmap(QString(":/pieces/") + piece + ".png");
    square->setPixmap(pixmap.scaled(pieceSize, pieceSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void setPieceImage(const int piece_int, QPushButton *btn) {
    const char *piece = PieceImages[piece_int].data();
    QPixmap pixmap(QString(":/pieces/") + piece + ".png");
    btn->setIcon(QIcon(pixmap.scaled(pieceSize, pieceSize, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    btn->setIconSize(QSize(pieceSize, pieceSize));
    btn->setFixedSize(QSize(pieceSize, pieceSize));
}

void updatePieceImages(const Array8x8<int> &board, Array8x8<DraggableLabel *> &squares) {
    for (int row=0; row<8; ++row) {
        for (int col=0; col<8; ++col) {
            int piece = board[row][col];
            if (piece != -1) {
                setPieceImage(piece, squares[row][col]);
            } else {
                squares[row][col]->clear();
            }
        }
    }
}


void MainWindow::enpassant(int move) {
    int enpassant = get_enpassant_flag(move);
    int dest = get_move_target(move);
    const int StartOfRank6 = 40;
    if (enpassant != 0) {
        int taken_pawn;
        if (dest >= StartOfRank6) {
            taken_pawn = dest - 8;
        } else {
            taken_pawn = dest + 8;
        }
        squares[taken_pawn / 8][taken_pawn % 8]->setPixmap(QPixmap());
    }
}

void MainWindow::engineTurn(int move) {
    print_move(move);

    int before_turn = game.board.turn;

    bool king_safe = make_move(game.board, move);

    if (move == 0) {
        king_safe = 0;
    }

    if (!king_safe) {
        if (is_king_exposed(game.board, !player_color)) {
            printf("Checkmate! You win!\n");
        } else {
            printf("Stalemate!\n");
        }
        DDState->enabled = false;
    }

    int source = get_move_source(move);
    int dest = get_move_target(move);

    DraggableLabel *sourceSquare = squares[source / 8][source % 8];
    DraggableLabel *destSquare = squares[dest / 8][dest % 8];
    
    // need to change image on pawn promotions
    const QPixmap pieceImage = sourceSquare->pixmap(Qt::ReturnByValue);
    if (!pieceImage.isNull()) {
        destSquare->setPixmap(pieceImage);
        sourceSquare->setPixmap(QPixmap()); 
    } else {
        printf("source: %d is empty\n", source);
    }

    int prom_piece = get_promotion(move);
    if (prom_piece) {
        setPieceImage(prom_piece, destSquare);
    }

    enpassant(move);
    
    Board b = game.board;
    Searcher S;
    int pot_user_move = S.negamax(b, 1);
    bool user_has_legal_moves = make_move(b, pot_user_move);
    if (!user_has_legal_moves) {
        if (is_king_exposed(game.board, player_color)) {
            printf("Checkmate! I Win!\n");
        } else {
            printf("Stalemate!\n");
        }
        DDState->enabled = false;
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


void MainWindow::makePopup(DraggableLabel *dest, std::function<void(int)> callback) {
    QDialog *popup = new QDialog(this);
    popup->setWindowFlags(Qt::Popup);  // closes when you click outside
    QHBoxLayout *layout = new QHBoxLayout(popup);

    int color = 0;
    if (dest->row == 7) {
        color = 6;
    }

    int pieces[] = {
        BlackKnight + color,
        BlackBishop + color,
        BlackQueen + color
    };

    for (int i=0; i<3; i++) {
        QPushButton *btn = new QPushButton(popup);
        btn->setCursor(Qt::PointingHandCursor);

        int piece = pieces[i];

        QObject::connect(btn, &QPushButton::clicked, [popup, callback, piece]() {
            callback(piece);
            popup->accept();
        });

        setPieceImage(piece, btn);
        layout->addWidget(btn);
    }

    //popup->move(targetPosition);
    popup->exec();  // blocks until dismissed, or use show() if you don't want blocking
}

void MainWindow::getPlayerMove(GuiMove guimove) {
    printf("Callback\n");

    std::vector<int> move_list;

    generate_moves(game.board, move_list);

    int userSource = guimove.source->row * 8 + guimove.source->col;
    int userDest = guimove.dest->row * 8 + guimove.dest->col;
    int userMove = findUserMove(userSource, userDest, move_list);

    if(get_promotion(userMove)) {
        // promotion popup
        makePopup(
            guimove.dest,
            [this, userMove, guimove](int promotion) {
                playerMove(userMove, promotion, guimove);
            }
        );
    } else {
        playerMove(userMove, 0, guimove);
    }
}


void MainWindow::playerMove(int move, int promotion, GuiMove guimove) {
    const int promotion_mask = ~0xF0000;
    constexpr int PROMOTION_SHIFT = 16;

    int prom_move = (move & promotion_mask) | (promotion << PROMOTION_SHIFT);
    
    if (prom_move) {
        // user move is psuedolegal!

        Board new_board = game.board;
        bool king_safe = make_move(new_board, prom_move);
        if (king_safe) {
            // user move is legal!

            game.board = new_board;

            if (promotion) {
                setPieceImage(promotion, guimove.dest);
            }

            enpassant(move);

            RequestEngineMove(game);
            return;
        }
    }

    // move illegal because it leaves the king vulnerable
    const QPixmap pieceImage = guimove.dest->pixmap(Qt::ReturnByValue);
    if (!pieceImage.isNull()) {
        guimove.source->setPixmap(pieceImage);
        guimove.dest->setPixmap(guimove.taken_piece); 
    } else {
        printf(
            "While trying to undo illegal move: source %d is empty\n",
            guimove.source->row * 8 + guimove.source->col
        );
    }

    printf("User made illegal move from row %d col %d to row %d col %d\n",
        guimove.source->row, guimove.source->col,
        guimove.dest->row, guimove.dest->col);
}



Array8x8<DraggableLabel*> MainWindow::setupChessboard(QWidget *parent) {
    QGridLayout *layout = new QGridLayout(parent);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    Array8x8<DraggableLabel*> squares;

    auto boundMethod = [this](GuiMove move) {
        this->getPlayerMove(move);
    };

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            DraggableLabel *square = new DraggableLabel(
                boundMethod,
                this->DDState,
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

            layout->addWidget(square, 7-row, col+1);
        }

        QLabel *row_label = new QLabel(QString(QChar('a' + row)), parent);
        row_label->setFixedSize(60, 60);
        row_label->setAlignment(Qt::AlignCenter);
        layout->addWidget(row_label, 8, row + 1);  // bottom row

        QLabel *col_label = new QLabel(QString::number(row + 1), parent);
        col_label->setFixedSize(60, 60);
        col_label->setAlignment(Qt::AlignCenter);
        layout->addWidget(col_label, 7 - row, 0);  // left column
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

    DDState = std::make_shared<DragDropState>();

    QWidget *gridLayout = findChild<QWidget *>("gridLayoutWidget");
    gridLayout->setFixedSize(480, 480);
    if (gridLayout) {
        squares = setupChessboard(gridLayout);
    } else {
        qWarning("Grid layout widget not found in the UI.");
    }

    setupPieces(board, game.board);
    updatePieceImages(board, squares);

    qRegisterMetaType<Game>();

    thread = new QThread(this);
    EngineWorker *worker = new EngineWorker;

    worker->moveToThread(thread);
    thread->start();

    connect(this, &MainWindow::RequestEngineMove, worker, &EngineWorker::calculateMove);
    connect(worker, &EngineWorker::moveReady, this, &MainWindow::engineTurn);
}

MainWindow::~MainWindow() {
    if (thread) {
        thread->quit();   // asks the thread's event loop to stop
        thread->wait();   // blocks until the thread actually finishes
    }
    delete ui;
}

