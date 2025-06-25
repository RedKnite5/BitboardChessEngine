#include "mainwindow.h"

#include <QApplication>

#include "board.cpp"



int main(int argc, char *argv[])
{
    init_all();

    const char *position = "8/8/8/8/6R1/8/2P5/8 w - - 0 1";
    Board bd = Board(position);
    printBoard(bd);
    std::cout << "Attacked: " << is_square_attacked(bd, E1, white) << std::endl;

    printAllAttackedSquares(bd, white);





    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();
}
