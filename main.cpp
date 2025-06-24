#include "mainwindow.h"

#include <QApplication>

#include "board.cpp"



int main(int argc, char *argv[])
{
    //init_all();

    /*
    U64 blockers = 0ULL;
    blockers |= 1ULL << C4;
    blockers |= 1ULL << G7;
    printBitBoard(get_queen_attacks(D4, blockers));
    



    const char *position = "8/8/8/8/6R1/8/8/8 w - - 0 1";
    Board bd = Board(position);
    printBoard(bd);
    //std::cout << "Attacked: " << is_square_attacked(bd, E1, white) << std::endl;

    printAllAttackedSquares(bd, white);

    std::cout << "E4: " << is_square_attacked(bd, E4, white) << std::endl;
    printBitBoard(get_rook_attacks(E4, bd.allPieces));

    printBoard(bd);


    //printBoard(bd);
    */

    init_magic_numbers();
    
    


    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();
}
