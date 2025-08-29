#include <gtest/gtest.h>

#include "board.cpp"


long driver(Board &board, int depth) {
    if (depth == 0) {
        return 1;
    }
    long nodes = 0;

    std::vector<int> move_list;
    move_list.reserve(32);
    generate_moves(board, move_list);

    for (int move : move_list) {
        Board new_board = board;
        bool king_safe = make_move(new_board, move);
        if (!king_safe) {
            continue;
        }
        nodes += driver(new_board, depth-1);
    }
    return nodes;
}


std::array<char, 8> move_int_to_move_array(int move) {
    std::array<char, 8> ar;
    ar[0] = get_move_source(move);
    ar[1] = get_move_target(move);
    ar[2] = get_move_piece(move);
    ar[3] = get_promotion(move);
    ar[4] = (bool)get_capture_flag(move);
    ar[5] = (bool)get_double_push_flag(move);
    ar[6] = (bool)get_enpassant_flag(move);
    ar[7] = (bool)get_castle_flag(move);
    return ar;
}


void print_array(std::array<char, 8> ar) {
    printf("{%s, %s, %c, %c, %d, %d, %d, %d}",
        square_coords[ar[0]],
        square_coords[ar[1]],
        ascii_pieces[ar[2]],
        ascii_pieces[ar[3]],
        ar[4],
        ar[5],
        ar[6],
        ar[7]
    );
}

void print_move_as_array(int move) {
    print_array(move_int_to_move_array(move));
}

template <std::size_t... I>
int call_encode(const std::array<char, 8> &ar, std::index_sequence<I...>) {
    return encode_move(ar[I]...);
}

int call_encode(const std::array<char, 8> &ar) {
    return call_encode(ar, std::make_index_sequence<8>{});
}

bool COMPARE_MOVE_LIST_UNORDERED(std::vector<std::array<char, 8>> expected_array, std::vector<int> result) {
    std::vector<int> expected;
    for (auto um: expected_array) {
        expected.push_back(call_encode(um));
    }
    std::sort(expected.begin(), expected.end());
    std::sort(result.begin(), result.end());
    
    return result == expected;
}


TEST(HelloWorldTest, BasicAssertions) {
    EXPECT_EQ(1 + 1, 2);
}

TEST(Encoding, WhitePawnEncoding) {
    EXPECT_EQ(ascii_pieces[P], 'P');
}

TEST(Encoding, BlackPawnEncoding) {
    EXPECT_TRUE(ascii_pieces[p] == 'p');
}

TEST(Encoding, WhiteKnightEncoding) {
    EXPECT_TRUE(ascii_pieces[N] == 'N');
}

TEST(Encoding, BlackKnightEncoding) {
    EXPECT_TRUE(ascii_pieces[n] == 'n');
}

TEST(Encoding, WhiteBishopEncoding) {
    EXPECT_TRUE(ascii_pieces[B] == 'B');
}

TEST(Encoding, BlackBishopEncoding) {
    EXPECT_TRUE(ascii_pieces[b] == 'b');
}

TEST(Encoding, WhiteRookEncoding) {
    EXPECT_TRUE(ascii_pieces[R] == 'R');
}

TEST(Encoding, BlackRookEncoding) {
    EXPECT_TRUE(ascii_pieces[r] == 'r');
}

TEST(Encoding, WhiteQueenEncoding) {
    EXPECT_TRUE(ascii_pieces[Q] == 'Q');
}

TEST(Encoding, BlackQueenEncoding) {
    EXPECT_TRUE(ascii_pieces[q] == 'q');
}

TEST(Encoding, WhiteKingEncoding) {
    EXPECT_TRUE(ascii_pieces[K] == 'K');
}

TEST(Encoding, BlackKingEncoding) {
    EXPECT_TRUE(ascii_pieces[k] == 'k');
}


TEST(Encoding, SquareEncodingA1) {
    EXPECT_EQ(square_coords[A1][0], 'A');
    EXPECT_EQ(square_coords[A1][1], '1');
    EXPECT_EQ(square_coords[A1][2], '\0');
}

TEST(Encoding, SquareEncodingH1) {
    EXPECT_EQ(square_coords[H1][0], 'H');
    EXPECT_EQ(square_coords[H1][1], '1');
    EXPECT_EQ(square_coords[H1][2], '\0');
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

    std::vector<std::array<char, 8>> expected = {
        {A2, A3, P, p, 0, 0, 0, 0},
        {A2, A4, P, p, 0, 1, 0, 0},
        {B2, B3, P, p, 0, 0, 0, 0},
        {B2, B4, P, p, 0, 1, 0, 0},
        {C2, C3, P, p, 0, 0, 0, 0},
        {C2, C4, P, p, 0, 1, 0, 0},
        {D2, D3, P, p, 0, 0, 0, 0},
        {D2, D4, P, p, 0, 1, 0, 0},
        {E2, E3, P, p, 0, 0, 0, 0},
        {E2, E4, P, p, 0, 1, 0, 0},
        {F2, F3, P, p, 0, 0, 0, 0},
        {F2, F4, P, p, 0, 1, 0, 0},
        {G2, G3, P, p, 0, 0, 0, 0},
        {G2, G4, P, p, 0, 1, 0, 0},
        {H2, H3, P, p, 0, 0, 0, 0},
        {H2, H4, P, p, 0, 1, 0, 0},
        {A1, B1, R, p, 0, 0, 0, 0},
        {A1, C1, R, p, 0, 0, 0, 0},
        {A1, D1, R, p, 0, 0, 0, 0},
        {H1, F1, R, p, 0, 0, 0, 0},
        {H1, G1, R, p, 0, 0, 0, 0},
        {E1, D1, K, p, 0, 0, 0, 0},
        {E1, F1, K, p, 0, 0, 0, 0},
        {E1, C1, K, p, 0, 0, 0, 1},
        {E1, G1, K, p, 0, 0, 0, 1}
    };

    generate_moves(bd, res);
    EXPECT_TRUE(COMPARE_MOVE_LIST_UNORDERED(expected, res));
}

TEST(MoveGeneration, Castling2) {
    const char *position = "r3k2r/pppppppp/8/8/8/8/8/RN2K2R w Qkq - 0 1 ";
    Board bd = Board(position);
    std::vector<int> res;
    std::vector<std::array<char, 8>> expected = {
        {B1, D2, N, p, 0, 0, 0, 0},
        {B1, A3, N, p, 0, 0, 0, 0},
        {B1, C3, N, p, 0, 0, 0, 0},
        {A1, A2, R, p, 0, 0, 0, 0},
        {A1, A3, R, p, 0, 0, 0, 0},
        {A1, A4, R, p, 0, 0, 0, 0},
        {A1, A5, R, p, 0, 0, 0, 0},
        {A1, A6, R, p, 0, 0, 0, 0},
        {A1, A7, R, p, 1, 0, 0, 0},
        {H1, F1, R, p, 0, 0, 0, 0},
        {H1, G1, R, p, 0, 0, 0, 0},
        {H1, H2, R, p, 0, 0, 0, 0},
        {H1, H3, R, p, 0, 0, 0, 0},
        {H1, H4, R, p, 0, 0, 0, 0},
        {H1, H5, R, p, 0, 0, 0, 0},
        {H1, H6, R, p, 0, 0, 0, 0},
        {H1, H7, R, p, 1, 0, 0, 0},
        {E1, D1, K, p, 0, 0, 0, 0},
        {E1, F1, K, p, 0, 0, 0, 0},
        {E1, D2, K, p, 0, 0, 0, 0},
        {E1, E2, K, p, 0, 0, 0, 0},
        {E1, F2, K, p, 0, 0, 0, 0},
    };

    generate_moves(bd, res);
    EXPECT_TRUE(COMPARE_MOVE_LIST_UNORDERED(expected, res));
}


TEST(MoveGeneration, TrickyPositionWhite) {
    const char *position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
    Board bd = Board(position);
    std::vector<int> res;
    std::vector<std::array<char, 8>> expected = {
        {A2, A3, P, p, 0, 0, 0, 0},
        {A2, A4, P, p, 0, 1, 0, 0},
        {B2, B3, P, p, 0, 0, 0, 0},
        {G2, G3, P, p, 0, 0, 0, 0},
        {G2, G4, P, p, 0, 1, 0, 0},
        {G2, H3, P, p, 1, 0, 0, 0},
        {D5, D6, P, p, 0, 0, 0, 0},
        {D5, E6, P, p, 1, 0, 0, 0},
        {C3, B1, N, p, 0, 0, 0, 0},
        {C3, D1, N, p, 0, 0, 0, 0},
        {C3, A4, N, p, 0, 0, 0, 0},
        {C3, B5, N, p, 0, 0, 0, 0},
        {E5, D3, N, p, 0, 0, 0, 0},
        {E5, C4, N, p, 0, 0, 0, 0},
        {E5, G4, N, p, 0, 0, 0, 0},
        {E5, C6, N, p, 0, 0, 0, 0},
        {E5, G6, N, p, 1, 0, 0, 0},
        {E5, D7, N, p, 1, 0, 0, 0},
        {E5, F7, N, p, 1, 0, 0, 0},
        {D2, C1, B, p, 0, 0, 0, 0},
        {D2, E3, B, p, 0, 0, 0, 0},
        {D2, F4, B, p, 0, 0, 0, 0},
        {D2, G5, B, p, 0, 0, 0, 0},
        {D2, H6, B, p, 0, 0, 0, 0},
        {E2, D1, B, p, 0, 0, 0, 0},
        {E2, F1, B, p, 0, 0, 0, 0},
        {E2, D3, B, p, 0, 0, 0, 0},
        {E2, C4, B, p, 0, 0, 0, 0},
        {E2, B5, B, p, 0, 0, 0, 0},
        {E2, A6, B, p, 1, 0, 0, 0},
        {A1, B1, R, p, 0, 0, 0, 0},
        {A1, C1, R, p, 0, 0, 0, 0},
        {A1, D1, R, p, 0, 0, 0, 0},
        {H1, F1, R, p, 0, 0, 0, 0},
        {H1, G1, R, p, 0, 0, 0, 0},
        {F3, D3, Q, p, 0, 0, 0, 0},
        {F3, E3, Q, p, 0, 0, 0, 0},
        {F3, G3, Q, p, 0, 0, 0, 0},
        {F3, H3, Q, p, 1, 0, 0, 0},
        {F3, F4, Q, p, 0, 0, 0, 0},
        {F3, G4, Q, p, 0, 0, 0, 0},
        {F3, F5, Q, p, 0, 0, 0, 0},
        {F3, H5, Q, p, 0, 0, 0, 0},
        {F3, F6, Q, p, 1, 0, 0, 0},
        {E1, D1, K, p, 0, 0, 0, 0},
        {E1, F1, K, p, 0, 0, 0, 0},
        {E1, C1, K, p, 0, 0, 0, 1},
        {E1, G1, K, p, 0, 0, 0, 1},
    };

    generate_moves(bd, res);
    EXPECT_TRUE(COMPARE_MOVE_LIST_UNORDERED(expected, res));
}

TEST(MoveGeneration, TrickyPositionBlack) {
    const char *position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1 ";
    Board bd = Board(position);
    std::vector<int> res;

    std::vector<std::array<char, 8>> expected = {
        {H3, G2, p, p, 1, 0, 0, 0},
        {B4, B3, p, p, 0, 0, 0, 0},
        {B4, C3, p, p, 1, 0, 0, 0},
        {E6, D5, p, p, 1, 0, 0, 0},
        {G6, G5, p, p, 0, 0, 0, 0},
        {C7, C5, p, p, 0, 1, 0, 0},
        {C7, C6, p, p, 0, 0, 0, 0},
        {D7, D6, p, p, 0, 0, 0, 0},
        {B6, A4, n, p, 0, 0, 0, 0},
        {B6, C4, n, p, 0, 0, 0, 0},
        {B6, D5, n, p, 1, 0, 0, 0},
        {B6, C8, n, p, 0, 0, 0, 0},
        {F6, E4, n, p, 1, 0, 0, 0},
        {F6, G4, n, p, 0, 0, 0, 0},
        {F6, D5, n, p, 1, 0, 0, 0},
        {F6, H5, n, p, 0, 0, 0, 0},
        {F6, H7, n, p, 0, 0, 0, 0},
        {F6, G8, n, p, 0, 0, 0, 0},
        {A6, E2, b, p, 1, 0, 0, 0},
        {A6, D3, b, p, 0, 0, 0, 0},
        {A6, C4, b, p, 0, 0, 0, 0},
        {A6, B5, b, p, 0, 0, 0, 0},
        {A6, B7, b, p, 0, 0, 0, 0},
        {A6, C8, b, p, 0, 0, 0, 0},
        {G7, H6, b, p, 0, 0, 0, 0},
        {G7, F8, b, p, 0, 0, 0, 0},
        {A8, B8, r, p, 0, 0, 0, 0},
        {A8, C8, r, p, 0, 0, 0, 0},
        {A8, D8, r, p, 0, 0, 0, 0},
        {H8, H4, r, p, 0, 0, 0, 0},
        {H8, H5, r, p, 0, 0, 0, 0},
        {H8, H6, r, p, 0, 0, 0, 0},
        {H8, H7, r, p, 0, 0, 0, 0},
        {H8, F8, r, p, 0, 0, 0, 0},
        {H8, G8, r, p, 0, 0, 0, 0},
        {E7, C5, q, p, 0, 0, 0, 0},
        {E7, D6, q, p, 0, 0, 0, 0},
        {E7, D8, q, p, 0, 0, 0, 0},
        {E7, F8, q, p, 0, 0, 0, 0},
        {E8, D8, k, p, 0, 0, 0, 0},
        {E8, F8, k, p, 0, 0, 0, 0},
        {E8, C8, k, p, 0, 0, 0, 1},
        {E8, G8, k, p, 0, 0, 0, 1},
    };

    generate_moves(bd, res);
    EXPECT_TRUE(COMPARE_MOVE_LIST_UNORDERED(expected, res));
}



TEST(MoveGeneration, TestEnpassant1) {
    const char *position = "k7/7P/8/3pP3/8/K7/8/8 w - d6 0 1";
    Board bd = Board(position);
    std::vector<int> res;

    std::vector<std::array<char, 8>> expected = {
        {E5, E6, P, p, 0, 0, 0, 0},
        {E5, D6, P, p, 1, 0, 1, 0},
        {H7, H8, P, Q, 0, 0, 0, 0},
        {H7, H8, P, B, 0, 0, 0, 0},
        {H7, H8, P, N, 0, 0, 0, 0},
        {H7, H8, P, R, 0, 0, 0, 0},
        {A3, A2, K, p, 0, 0, 0, 0},
        {A3, B2, K, p, 0, 0, 0, 0},
        {A3, B3, K, p, 0, 0, 0, 0},
        {A3, A4, K, p, 0, 0, 0, 0},
        {A3, B4, K, p, 0, 0, 0, 0},
    };

    generate_moves(bd, res);
    EXPECT_TRUE(COMPARE_MOVE_LIST_UNORDERED(expected, res));
}


TEST(Perft, PerftDepth2) {
    Board b = Board(start_position);
    auto nodes = driver(b, 2);
    EXPECT_EQ(nodes,  400);
}

TEST(Perft, PerftDepth3) {
    Board b = Board(start_position);
    auto nodes = driver(b, 3);
    EXPECT_EQ(nodes,  8902);
}

TEST(Perft, PerftDepth4) {
    Board b = Board(start_position);
    auto nodes = driver(b, 4);
    EXPECT_EQ(nodes,  197281);
}

TEST(Perft, PerftDepth5) {
    Board b = Board(start_position);
    auto nodes = driver(b, 5);
    EXPECT_EQ(nodes,  4865609);
}


TEST(MoveOrdering, TestMVV_LVA_Pp) {
    Board board = Board("rnbqkbnr/ppppppp1/8/7p/6P1/8/PPPPPP1P/RNBQKBNR w KQkq - 0 1 ");
    std::array<char, 8> move = {G4, H5, P, p, 1, 0, 0, 0};
    EXPECT_EQ(mvv_lva(board, call_encode(move)), 105);
}

TEST(MoveOrdering, TestMVV_LVA_pP) {
    Board board = Board("rnbqkbnr/ppppppp1/8/7p/6P1/8/PPPPPP1P/RNBQKBNR b KQkq - 0 1 ");
    std::array<char, 8> move = {H5, G4, p, p, 1, 0, 0, 0};
    EXPECT_EQ(mvv_lva(board, call_encode(move)), 105);
}

TEST(MoveOrdering, TestMVV_LVA_Pq) {
    Board board = Board("rnb1kbnr/pppppppp/8/7q/6P1/8/PPPPPP1P/RNBQKBNR w KQkq - 0 1 ");
    std::array<char, 8> move = {G4, H5, P, p, 1, 0, 0, 0};
    EXPECT_EQ(mvv_lva(board, call_encode(move)), 505);
}

TEST(MoveOrdering, TestMVV_LVA_Qp) {
    Board board = Board("rnbqkbnr/ppppppp1/8/7p/6Q1/8/PPPPPP1P/RNBQKBNR b KQkq - 0 1 ");
    std::array<char, 8> move = {G4, H5, Q, p, 1, 0, 0, 0};
    EXPECT_EQ(mvv_lva(board, call_encode(move)), 101);
}



int main(int argc, char **argv) {

    // std::vector<int> expected = {27428, 5270244, 684023, 618487, 552951, 487415, 45584, 45648, 46160, 46608, 46672};
    
    // for (int move: expected) {
    //     print_move_as_array(move);
    //     printf(", \n");
    // }



    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
