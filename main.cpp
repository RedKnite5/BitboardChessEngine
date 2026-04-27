
#include <QApplication>
#include "mainwindow.h"
#include "search.h"

int play_graphical(int argc, char **argv, const char *fen, int moves, int depth) {
    Game game(fen);

    printBoard(game.board);


    QApplication a(argc, argv);
    MainWindow w(game);
    w.show();
    return a.exec();
}


int main(int argc, char **argv) {

    printf("Start!\n");

    //const char *fen = "1rb5/1p2k2r/p5n1/2p1pp2/2B5/6P1/PPPB1PP1/2KR4 w - - 1 0";

    const char *fen = "8/1pk5/p7/2p4P/2B5/6P1/PPPB1PP1/2KR4 w - - 1 0";
    const char *fen2 = "r3k3/pbpqb1r1/1p2Q1p1/3pP1B1/3P4/3B4/PPP4P/5RK1 w - - 1 0";

    return play_graphical(argc, argv, fen2, 15, 8);
}

