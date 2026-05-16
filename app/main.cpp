
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
        QTimer::singleShot(100, &w, [&w, game]() {
            w.RequestEngineMove(game);
        });
    }

    return a.exec();
}


int main(int argc, char **argv) {
    // must match the .qrc filename (resources.qrc)
    Q_INIT_RESOURCE(resources);

    printf("Start!\n");

    // Useful positions:
    //const char *allow_me_to_enpassant = "r1bqk1nr/p1p2ppp/8/3P4/1RPRP3/6P1/P2P3P/2BQK1NR b KQkq e3 0 1";
    //const char *computer_about_to_win = "r3k3/pbpqb1r1/1p2Q1p1/3pP1B1/3P4/3B4/PPP4P/5RK1 w - - 1 0";


    //const char *fen = "1rb5/1p2k2r/p5n1/2p1pp2/2B5/6P1/PPPB1PP1/2KR4 w - - 1 0";

    //const char *user_about_to_promote_and_win = "8/1pk5/p6P/2p3P1/2B5/8/PPPB1PP1/2KR4 w - - 1 0";

    //const char *castling = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1 ";

    const char *start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";
    

    return play_graphical(argc, argv, start, 15, 9);
}

