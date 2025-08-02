#include <gtest/gtest.h>

#include "board.cpp"

TEST(HelloWorldTest, BasicAssertions) {
    EXPECT_EQ(1 + 1, 2);
}

TEST(StartingBoard, WhitePieces) {
    std::string whiteBitboardString =
        "8  . . . . . . . . \n"
        "7  . . . . . . . . \n"
        "6  . . . . . . . . \n"
        "5  . . . . . . . . \n"
        "4  . . . . . . . . \n"
        "3  . . . . . . . . \n"
        "2  1 1 1 1 1 1 1 1 \n"
        "1  1 1 1 1 1 1 1 1 \n"
        "\n"
        "   A B C D E F G H\n"
        "65535\n";
    std::stringstream s;
    Board bd = Board();
    printBitBoard_stream(bd.coloredPieces[WHITE], s);
    
    EXPECT_EQ(s.str(), whiteBitboardString);
}

TEST(StartingBoard, BlackPieces) {
    std::string blackBitboardString =
        "8  1 1 1 1 1 1 1 1 \n"
        "7  1 1 1 1 1 1 1 1 \n"
        "6  . . . . . . . . \n"
        "5  . . . . . . . . \n"
        "4  . . . . . . . . \n"
        "3  . . . . . . . . \n"
        "2  . . . . . . . . \n"
        "1  . . . . . . . . \n"
        "\n"
        "   A B C D E F G H\n"
        "18446462598732840960\n";
    std::stringstream s;
    Board bd = Board();
    printBitBoard_stream(bd.coloredPieces[BLACK], s);
    
    EXPECT_EQ(s.str(), blackBitboardString);
}

TEST(StartingBoard, WhitePawns) {
    U64 whitePawns = 65280;
    Board bd = Board();
    EXPECT_EQ(bd.bitboards[P], whitePawns);
}



TEST(SlidingAttacks, Rook1) {
    U64 blockers = (1ULL << E2) | (1ULL << E6);
    U64 res = get_rook_attacks(E4, blockers);
    EXPECT_EQ(res, 17664916328448);
}

TEST(SlidingAttacks, Rook2) {
    U64 blockers = (1ULL << E2) | (1ULL << E6) | (1ULL << E5);
    U64 res = get_rook_attacks(E4, blockers);
    EXPECT_EQ(res, 72730284032);
}



TEST(SlidingAttacks, Bishop1) {
    U64 blockers = (1ULL << G2) | (1ULL << E6) | (1ULL << D5);
    U64 res = get_bishop_attacks(E4, blockers);
    EXPECT_EQ(res, 36099337564472322);
}


TEST(SlidingAttacks, Queen1) {
    U64 blockers = (1ULL << G2) | (1ULL << E6) | (1ULL << D5) | (1ULL << E2) | (1ULL << E6) | (1ULL << E5) | (1ULL << B7);
    U64 res = get_queen_attacks(E4, blockers);
    EXPECT_EQ(res, 36099410294756354);
}




TEST(AllSquaresAttacked, StartingPositionWhite) {
    std::string allSquaresAttacked =
        "8  . . . . . . . . \n"
        "7  . . . . . . . . \n"
        "6  . . . . . . . . \n"
        "5  . . . . . . . . \n"
        "4  . . . . . . . . \n"
        "3  X X X X X X X X \n"
        "2  X X X X X X X X \n"
        "1  . X X X X X X . \n"
        "\n"
        "   A B C D E F G H\n";
    std::stringstream s;
    Board bd = Board();
    printAllAttackedSquares_stream(bd, WHITE, s);
    
    EXPECT_EQ(s.str(), allSquaresAttacked);
}


TEST(MoveGeneration, Castling1) {
    const char *position = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1 ";
    Board bd = Board(position);
    std::vector<int> res;
    std::vector<int> expected = {25608, 2123272, 25673, 2123337, 25738, 2123402,
        25803, 2123467, 25868, 2123532, 25933, 2123597, 25998, 2123662, 26063,
        2123727, 28736, 28800, 28864, 28999, 29063, 45252, 45380, 8433796, 8434052};
    generate_moves(bd, res);
    EXPECT_EQ(res, expected);
}

TEST(MoveGeneration, Castling2) {
    const char *position = "r3k2r/pppppppp/8/8/8/8/8/RN2K2R w Qkq - 0 1 ";
    Board bd = Board(position);
    std::vector<int> res;
    std::vector<int> expected = {33473, 33793, 33921, 29184, 29696, 30208, 30720,
        31232, 1080320, 28999, 29063, 29639, 30151, 30663, 31175, 31687, 1080775,
        45252, 45380, 45764, 45828, 45892};
    generate_moves(bd, res);
    EXPECT_EQ(res, expected);
}


TEST(MoveGeneration, TrickyPositionWhite) {
    const char *position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
    Board bd = Board(position);
    std::vector<int> res;
    std::vector<int> expected = {25608, 2123272, 25673, 25998, 2123662, 1074638,
        27363, 1076003, 32850, 32978, 34322, 34898, 34020, 34468, 34724, 35492,
        1084324, 1084644, 1084772, 37003, 38155, 38731, 39307, 39883, 37068,
        37196, 38092, 38540, 38988, 1088012, 28736, 28800, 28864, 28999, 29063,
        42197, 42261, 42389, 1091029, 42837, 42901, 43349, 43477, 1092437, 45252,
        45380, 8433796, 8434052};
    generate_moves(bd, res);
    EXPECT_EQ(res, expected);
}

TEST(MoveGeneration, TrickyPositionBlack) {
    const char *position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1 ";
    Board bd = Board(position);
    std::vector<int> res;
    std::vector<int> expected = {1049495, 1113, 1049753, 1050860, 2478, 2099378,
        2738, 2803, 9769, 9897, 1059049, 11945, 1058605, 10157, 1059053, 10733,
        11757, 12205, 1061672, 13544, 13992, 14440, 15464, 16040, 15350, 16246,
        7800, 7864, 7928, 6143, 6655, 7167, 7679, 8063, 8127, 18612, 19188, 20212,
        20340, 24316, 24444, 8412860, 8413116};
    generate_moves(bd, res);
    EXPECT_EQ(res, expected);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
