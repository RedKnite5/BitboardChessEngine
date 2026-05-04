
#include <QApplication>
#include <QTimer>
#include "mainwindow.h"
#include "search.h"



int play_graphical(int argc, char **argv, const char *fen, int moves, int depth) {
    Game game(fen);

    printBoard(game.board);


    QApplication a(argc, argv);
    MainWindow w(game);
    w.show();

    if (game.board.turn == 0) {
        QTimer::singleShot(100, &w, &MainWindow::engineTurn);
    }

    return a.exec();
}


int main(int argc, char **argv) {
    // must match the .qrc filename (resources.qrc)
    Q_INIT_RESOURCE(resources);

    printf("Start!\n");

    //const char *fen = "1rb5/1p2k2r/p5n1/2p1pp2/2B5/6P1/PPPB1PP1/2KR4 w - - 1 0";

    //const char *fen = "8/1pk5/p7/2p4P/2B5/6P1/PPPB1PP1/2KR4 w - - 1 0";
    //const char *fen2 = "r3k3/pbpqb1r1/1p2Q1p1/3pP1B1/3P4/3B4/PPP4P/5RK1 w - - 1 0";
    //const char *fen3 = "rnbqkbnr/p1ppppppp/8/4P3/1p7/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";
    const char *fen3 = "rnbqkbnr/p1pppppp/8/4P3/1p6/8/PRPP1PPP/1NBQKBNR b KQkq e3 0 1";

    return play_graphical(argc, argv, fen3, 15, 8);
}

