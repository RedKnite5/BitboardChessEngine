#include "mainwindow.h"

#include <QApplication>

#include "board.cpp"



int main(int argc, char *argv[])
{
    init_all();

    //const char *position = "5B2/8/8/8/2P3R1/8/8/8 w - - 0 1";
    //const char *position = tricky_position;
    const char *position = "2q5/3P4/8/6pP/8/r1b5/1P6/8 w - g6 0 1";
    Board bd = Board(position);
    //printBoard(bd);
    //std::cout << "Attacked: " << is_square_attacked(bd, E1, white) << std::endl;

    printAllAttackedSquares(bd, white);


    std::vector<int> move_list;

    generate_moves(bd, move_list);

    print_move_list(move_list);

    printBoard(bd);



    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();
}
