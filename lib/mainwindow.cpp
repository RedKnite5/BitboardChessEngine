#include <memory>
#include <vector>
#include <time.h>

#include <QGridLayout>
#include <QDialog>
#include <QThread>
#include <QMetaType>

#include <QDebug>
#include <QElapsedTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "draggable.h"
#include "search.h"
#include "board.h"
#include "engine_worker.h"

template <typename T>
using Array8x8 = std::array<std::array<T, 8>, 8>;

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

constexpr int pieceSize = 50;

constexpr int get_lsb_index(unsigned long long x) {
    return __builtin_ctzll(x);
}

constexpr int get_lsb_index_safe(uint64_t bitBoard) {
    // returns -1 if input is 0
    int is_zero = bitBoard == 0;
    int safe_ctz = get_lsb_index(bitBoard | is_zero);
    return safe_ctz - is_zero;
}

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

void updatePieceImages(const std::array<int, 64> &board, std::array<DraggableLabel *, 64> &squares) {
    for (int s=0; s<64; ++s) {
        int piece = board[s];
        if (piece != -1) {
            setPieceImage(piece, squares[s]);
        } else {
            squares[s]->clear();
        }
    }
}


void MainWindow::enpassant(Move move) {
    int enpassant = move.get_enpassant_flag();
    int dest = move.get_move_target();
    const int StartOfRank6 = A6;  // 40
    if (enpassant != 0) {
        int taken_pawn;
        if (dest >= StartOfRank6) {
            taken_pawn = dest - 8;
        } else {
            taken_pawn = dest + 8;
        }
        squares[taken_pawn]->setPixmap(QPixmap());
    }
}

void castle_move_rook(std::array<DraggableLabel*, 64> squares, int from, int to) {
    QPixmap rook_image = squares[from]->pixmap(Qt::ReturnByValue);
    squares[to]->setPixmap(rook_image);
    squares[from]->setPixmap(QPixmap());
}

void MainWindow::castle(Move move) {
    int castle = move.get_castle_flag();
    if (!castle) {
        return;
    }

    int dest = move.get_move_target();
    switch (dest) {
        case C1:
            // A1 to D1
            castle_move_rook(squares, A1, D1);
            break;
        case G1:
            // H1 to F1
            castle_move_rook(squares, H1, F1);
            break;
        case C8:
            // A8 to D8
            castle_move_rook(squares, A8, D8);
            break;
        case G8:
            // H8 to F8
            castle_move_rook(squares, H8, F8);
            break;
        default:
            printf("Error: Castling to %d\n", dest);
    }
}

void MainWindow::clearHighlights() {
    int square = get_lsb_index_safe(highlighted);
    while (square >= 0) {
        //printf("square: %d\n", square);
        QPalette palette = squares[square]->palette();
        QColor color = ((square + square/8) % 2 == 1) ? Qt::white : Qt::gray;
        palette.setColor(QPalette::Window, color);
        squares[square]->setPalette(palette);

        highlighted &= highlighted - 1;
        square = get_lsb_index_safe(highlighted);
    }
}

void MainWindow::highlight(int square) {
    QPalette palette = squares[square]->palette();
    palette.setColor(QPalette::Window, QColor(255, 253, 208));
    squares[square]->setPalette(palette);
    highlighted |= 1ULL << square;
}

void MainWindow::engineTurn(int int_move) {
    Move move = Move(int_move);
    move.print_move();
    // need to test stalemate detenction more 5/11/26

    bool king_safe = game->board.make_move(move);

    if (move == Move(0)) {
        king_safe = 0;
    }

    if (!king_safe) {
        if (is_king_exposed(game->board, !player_color)) {
            printf("Checkmate! You win!\n");
        } else {
            printf("Stalemate!\n");
        }
        DDState->enabled = false;
    }

    int source = move.get_move_source();
    int dest = move.get_move_target();

    DraggableLabel *sourceSquare = squares[source];
    DraggableLabel *destSquare = squares[dest];
    
    // need to change image on pawn promotions
    const QPixmap pieceImage = sourceSquare->pixmap(Qt::ReturnByValue);
    if (!pieceImage.isNull()) {
        destSquare->setPixmap(pieceImage);
        sourceSquare->setPixmap(QPixmap()); 
    } else {
        printf("source: %d is empty\n", source);
    }

    int prom_piece = move.get_promotion();
    if (prom_piece) {
        setPieceImage(prom_piece, destSquare);
    }

    enpassant(move);
    castle(move);

    clearHighlights();
    highlight(source);
    highlight(dest);
    
    Board b = game->board;
    Searcher S;
    Move pot_user_move = S.negamax(b, 1);
    bool user_has_legal_moves = b.make_move(pot_user_move);
    if (!user_has_legal_moves) {
        if (is_king_exposed(game->board, player_color)) {
            printf("Checkmate! I Win!\n");
        } else {
            printf("Stalemate!\n");
        }
        DDState->enabled = false;
    }
}


Move findUserMove(int userSource, int userDest, const std::vector<Move> &move_list) {
    for (auto move : move_list) {
        int source_sq = move.get_move_source();
        int target_sq = move.get_move_target();

        if (source_sq == userSource && userDest == target_sq) {
            return move;
        }
    }
    return Move(0);
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
        n + color,
        b + color,
        q + color
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
    std::vector<Move> move_list;
    generate_moves(game->board, move_list);

    int userSource = guimove.source->row * 8 + guimove.source->col;
    int userDest = guimove.dest->row * 8 + guimove.dest->col;
    Move userMove = findUserMove(userSource, userDest, move_list);

    if(userMove.get_promotion()) {
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


void MainWindow::playerMove(Move move, int promotion, GuiMove guimove) {
    const int promotion_mask = ~0xF0000;
    constexpr int PROMOTION_SHIFT = 16;

    Move prom_move = Move((move.m & promotion_mask) | (promotion << PROMOTION_SHIFT));
    
    if (prom_move) {
        // user move is psuedolegal!

        Board new_board = game->board;
        bool king_safe = new_board.make_move(prom_move);
        if (king_safe) {
            // user move is legal!

            game->board = new_board;

            if (promotion) {
                setPieceImage(promotion, guimove.dest);
            }

            enpassant(move);
            castle(move);

            clearHighlights();
            highlight(move.get_move_source());
            highlight(move.get_move_target());

            RequestEngineMove(game, depth);
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



std::array<DraggableLabel*, 64> MainWindow::setupChessboard(QWidget *parent) {
    QGridLayout *layout = new QGridLayout(parent);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    std::array<DraggableLabel*, 64> squares;

    auto boundMethod = [this](GuiMove move) {
        this->getPlayerMove(move);
    };

    for (int s = 0; s < 64; ++s) {
        DraggableLabel *square = new DraggableLabel(
            boundMethod,
            this->DDState,
            parent);
        square->setFixedSize(60, 60);
        square->setAlignment(Qt::AlignCenter);

        QColor color = ((s + s/8) % 2 == 1) ? Qt::white : Qt::gray;
        QPalette palette = square->palette();
        palette.setColor(QPalette::Window, color);
        square->setAutoFillBackground(true);
        square->setPalette(palette);

        square->setObjectName(QString("square_%1").arg(s + 1));
        squares[s] = square;
        square->row = s / 8;
        square->col = s % 8;

        layout->addWidget(square, 7-square->row, square->col+1);
    }


    for (int row = 0; row < 8; ++row) {
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

void setPieceOnSquare(int i, std::array<int, 64> &board, Board &bitboard) {
    for (Piece piece=p; piece<=K; ++piece) {
        if (bitboard.bitboards[piece] & (1ULL << i)) {
            board[i] = piece;
            return;
        }
    }
}

void setupPieces(std::array<int, 64> &board, Board &bitboard) {
    for (int i=0; i<64; ++i) {
        if ((bitboard.allPieces & (1ULL << i)) == 0) {
            continue;
        }
        setPieceOnSquare(i, board, bitboard);
    }
}

//Q_DECLARE_METATYPE(std::shared_ptr<Game>)

MainWindow::MainWindow(std::shared_ptr<Game> aGame, int aDepth, QWidget *parent)
    : QMainWindow(parent),
    game(aGame),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    depth = aDepth;

    std::array<int, 64> board;
    board.fill(-1);

    DDState = std::make_shared<DragDropState>();

    QWidget *gridLayout = findChild<QWidget *>("gridLayoutWidget");
    gridLayout->setFixedSize(480, 480);
    if (gridLayout) {
        squares = setupChessboard(gridLayout);
    } else {
        qWarning("Grid layout widget not found in the UI.");
    }

    setupPieces(board, game->board);
    updatePieceImages(board, squares);

    qRegisterMetaType<std::shared_ptr<Game>>();

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

