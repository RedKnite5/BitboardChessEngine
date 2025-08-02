#include "mainwindow.h"

#include <QApplication>

#include "board.cpp"



int main(int argc, char *argv[])
{
    //init_all();

    //const char *position = "5B2/8/8/8/2P3R1/8/8/8 w - - 0 1";
    const char *position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1 ";
    //const char *position = "2q5/3P4/8/6pP/8/r1b5/1P6/8 w - g6 0 1";
    Board bd = Board(position);
    //printBoard(bd);
    //std::cout << "Attacked: " << is_square_attacked(bd, E1, white) << std::endl;

    //Board bd = Board();
    


    printAllAttackedSquares(bd, BLACK);


    printBoard(bd);


    std::vector<int> move_list;

    generate_moves(bd, move_list);

    print_move_list(move_list);



    //U64 blockers = (1ULL << G2) | (1ULL << E6) | (1ULL << D5) | (1ULL << E2) | (1ULL << E6) | (1ULL << E5);
    //U64 res = get_queen_attacks(E4, blockers);
    //printBitBoard(res);






    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();
}
