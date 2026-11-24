#include <gtest/gtest.h>

#include <vector>
#include <unordered_map>
#include <random>

#include "board.h"
#include "board.cpp"


long driver(Board &board, int depth) {
    if (depth == 0) {
        return 1;
    }
    long nodes = 0;

    std::vector<Move> move_list;
    move_list.reserve(32);
    generate_moves(board, move_list);

    for (Move move : move_list) {
        Board new_board = board;
        bool king_safe = new_board.make_move(move);
        if (!king_safe) {
            continue;
        }
        nodes += driver(new_board, depth-1);
    }
    return nodes;
}


std::array<char, 8> move_int_to_move_array(Move move) {
    std::array<char, 8> ar;
    ar[0] = move.get_move_source();
    ar[1] = move.get_move_target();
    ar[2] = move.get_move_piece();
    ar[3] = move.get_promotion();
    ar[4] = (bool)move.get_capture_flag();
    ar[5] = (bool)move.get_double_push_flag();
    ar[6] = (bool)move.get_enpassant_flag();
    ar[7] = (bool)move.get_castle_flag();
    return ar;
}


void print_array(std::array<char, 8> ar) {
    printf("{%s, %s, %c, %c, %d, %d, %d, %d}",
        square_coords[static_cast<unsigned char>(ar[0])],
        square_coords[static_cast<unsigned char>(ar[1])],
        ascii_pieces[static_cast<unsigned char>(ar[2])],
        ascii_pieces[static_cast<unsigned char>(ar[3])],
        ar[4],
        ar[5],
        ar[6],
        ar[7]
    );
}

void print_move_as_array(Move move) {
    print_array(move_int_to_move_array(move));
}

template <std::size_t... I>
Move call_encode(const std::array<char, 8> &ar, std::index_sequence<I...>) {
    return encode_move(ar[I]...);
}

Move call_encode(const std::array<char, 8> &ar) {
    return call_encode(ar, std::make_index_sequence<8>{});
}

bool COMPARE_MOVE_LIST_UNORDERED(std::vector<std::array<char, 8>> expected_array, std::vector<Move> result) {
    std::vector<Move> expected;
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


TEST(ZHashing, HashStart) {
    Board bd = Board();
    U64 correct_hash = 3770636898827649812;
    U64 hash = bd.zHash();
    EXPECT_EQ(hash, correct_hash);
}

TEST(ZHashing, HashesDistinct) {
    std::mt19937 mt(69);

    std::unordered_map<U64, Board> map;
    std::vector<Move> move_list;

    for (int i=0; i<10000; i++) {
        std::uniform_int_distribution<int> dist(5, 60);
        int plys = dist(mt);
        Board bd = Board();
        for (int j=0; j<=plys; j++) {
            move_list.clear();
            generate_legal_moves(bd, move_list);
            if (move_list.size() == 0) {
                break;
            }

            std::uniform_int_distribution<int> vec_dist(0, move_list.size()-1);
            int random_index = vec_dist(mt);
            bd.make_move(move_list[random_index]);
        }
        U64 hash = bd.zHash();
        if (map.contains(hash)) {
            EXPECT_EQ(map[hash], bd);
        } else {
            map.insert({hash, bd});
        }
    }
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
    std::vector<Move> res;

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
    std::vector<Move> res;
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
    std::vector<Move> res;
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
    std::vector<Move> res;

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
    std::vector<Move> res;

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
    EXPECT_EQ(mvv_lva(board, call_encode(move)), 5 + 128 * 1 * MOVE_SORTING_SCALE);
}

TEST(MoveOrdering, TestMVV_LVA_pP) {
    Board board = Board("rnbqkbnr/ppppppp1/8/7p/6P1/8/PPPPPP1P/RNBQKBNR b KQkq - 0 1 ");
    std::array<char, 8> move = {H5, G4, p, p, 1, 0, 0, 0};
    EXPECT_EQ(mvv_lva(board, call_encode(move)), 5 + 128 * 1 * MOVE_SORTING_SCALE);
}

TEST(MoveOrdering, TestMVV_LVA_Pq) {
    Board board = Board("rnb1kbnr/pppppppp/8/7q/6P1/8/PPPPPP1P/RNBQKBNR w KQkq - 0 1 ");
    std::array<char, 8> move = {G4, H5, P, p, 1, 0, 0, 0};
    EXPECT_EQ(mvv_lva(board, call_encode(move)), 5 + 128 * 5 * MOVE_SORTING_SCALE);
}

TEST(MoveOrdering, TestMVV_LVA_Qp) {
    Board board = Board("rnbqkbnr/ppppppp1/8/7p/6Q1/8/PPPPPP1P/RNBQKBNR b KQkq - 0 1 ");
    std::array<char, 8> move = {G4, H5, Q, p, 1, 0, 0, 0};
    EXPECT_EQ(mvv_lva(board, call_encode(move)), 1 + 128 * MOVE_SORTING_SCALE);
}


void move_test(const char *start, Move move, const char *end) {
    Board bd = Board(start);
    bd.make_move(move);
    Board correct_result = Board(end);
    EXPECT_EQ(bd, correct_result);
    EXPECT_EQ(bd.hash_key, correct_result.zHash());
}

TEST(MakeMove, MovePawn) {
    Board bd = Board();
    Move move = encode_move(A2, A3, P, p, 0, 0, 0, 0);
    bd.make_move(move);
    Board correct_result = Board("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1");
    EXPECT_EQ(bd, correct_result);
    EXPECT_EQ(bd.hash_key, correct_result.zHash());
}

TEST(MakeMove, MoveRook) {
    move_test("rnbqkbnr/1ppppppp/p7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 0 1",
        encode_move(A1, A2, R, p, 0, 0, 0, 0),
        "rnbqkbnr/1ppppppp/p7/8/8/P7/RPPPPPPP/1NBQKBNR b Kkq - 0 1");
}

TEST(MakeMove, MoveKnight) {
    Board bd = Board();
    Move move = encode_move(B1, C3, N, p, 0, 0, 0, 0);
    bd.make_move(move);
    Board correct_result = Board("rnbqkbnr/pppppppp/8/8/8/2N5/PPPPPPPP/R1BQKBNR b KQkq - 0 1");
    EXPECT_EQ(bd, correct_result);
    EXPECT_EQ(bd.hash_key, correct_result.zHash());
}

TEST(MakeMove, MoveBishop) {
    move_test("rnbqkbnr/p1pppppp/1p6/8/8/1P6/P1PPPPPP/RNBQKBNR w KQkq - 0 1",
        encode_move(C1, B2, B, p, 0, 0, 0, 0),
        "rnbqkbnr/p1pppppp/1p6/8/8/1P6/PBPPPPPP/RN1QKBNR b KQkq - 0 1");
}

TEST(MakeMove, MoveQueen) {
    move_test("rnbqkbnr/1ppppppp/p7/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1",
        encode_move(D1, D2, Q, p, 0, 0, 0, 0),
        "rnbqkbnr/1ppppppp/p7/8/8/3P4/PPPQPPPP/RNB1KBNR b KQkq - 0 1");
}

TEST(MakeMove, MoveKing) {
    move_test("rnbqkbnr/1ppppppp/p7/8/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 1",
        encode_move(E1, E2, K, p, 0, 0, 0, 0),
        "rnbqkbnr/1ppppppp/p7/8/8/4P3/PPPPKPPP/RNBQ1BNR b kq - 0 1");
}

TEST(MakeMove, MoveBlackPawn) {
    move_test("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR b KQkq - 0 1",
        encode_move(D7, D6, p, p, 0, 0, 0, 0),
        "rnbqkbnr/ppp1pppp/3p4/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1");
}

TEST(MakeMove, MoveDoublePush) {
    Board bd = Board();
    Move move = encode_move(A2, A4, P, p, 0, 1, 0, 0);
    bd.make_move(move);
    Board correct_result = Board("rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1");
    EXPECT_EQ(bd, correct_result);
    EXPECT_EQ(bd.hash_key, correct_result.zHash());
}

TEST(MakeMove, MovePawnCapture) {
    move_test("rnbqkb1r/pppppppp/8/8/4n3/3P3P/PPP1PPP1/RNBQKBNR w KQkq - 0 1",
        encode_move(D3, E4, P, p, 1, 0, 0, 0),
        "rnbqkb1r/pppppppp/8/8/4P3/7P/PPP1PPP1/RNBQKBNR b KQkq - 0 1");
}

TEST(MakeMove, MoveEnPassant) {
    move_test("rnbqkbnr/p1pppppp/8/8/Pp6/6PP/1PPPPP2/RNBQKBNR b KQkq a3 0 1",
        encode_move(B4, A3, p, p, 1, 0, 1, 0),
        "rnbqkbnr/p1pppppp/8/8/8/p5PP/1PPPPP2/RNBQKBNR w KQkq - 0 1");
}

TEST(MakeMove, MoveEnPassant2) {
    move_test("rnbqkbnr/ppp1pppp/8/8/3pP3/6PP/PPPP1P2/RNBQKBNR b KQkq e3 0 1",
        encode_move(D4, E3, p, p, 1, 0, 1, 0),
        "rnbqkbnr/ppp1pppp/8/8/8/4p1PP/PPPP1P2/RNBQKBNR w KQkq - 0 1");
}

TEST(MakeMove, MoveWhiteEnPassant) {
    move_test("rnbqkbnr/p1ppppp1/7p/Pp6/8/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 1",
        encode_move(A5, B6, P, P, 1, 0, 1, 0),
        "rnbqkbnr/p1ppppp1/1P5p/8/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 1");
}

TEST(MakeMove, MoveIgnoreEnPassant) {
    move_test("rnbqkbnr/p1pppppp/8/8/Pp6/6PP/1PPPPP2/RNBQKBNR b KQkq a3 0 1",
        encode_move(C7, C6, p, p, 0, 0, 0, 0),
        "rnbqkbnr/p2ppppp/2p5/8/Pp6/6PP/1PPPPP2/RNBQKBNR w KQkq - 0 1");
}

TEST(MakeMove, MoveCastleKingSide) {
    move_test("rnbqkbnr/3ppppp/ppp5/8/8/4PN2/PPPPBPPP/RNBQK2R w KQkq - 0 1",
        encode_move(E1, G1, K, p, 0, 0, 0, 1),
        "rnbqkbnr/3ppppp/ppp5/8/8/4PN2/PPPPBPPP/RNBQ1RK1 b kq - 0 1");
}

TEST(MakeMove, MoveCastleQueenSide) {
    move_test("rnbqkbnr/3ppppp/ppp5/8/8/2NPB3/PPPQPPPP/R3KBNR w KQkq - 0 1",
        encode_move(E1, C1, K, p, 0, 0, 0, 1),
        "rnbqkbnr/3ppppp/ppp5/8/8/2NPB3/PPPQPPPP/2KR1BNR b kq - 0 1");
}

TEST(MakeMove, MoveCastleBlackKingSide) {
    move_test("rnbqk2r/ppppbppp/4pn2/8/8/PPPP4/4PPPP/RNBQKBNR b KQkq - 0 1",
        encode_move(E8, G8, k, p, 0, 0, 0, 1),
        "rnbq1rk1/ppppbppp/4pn2/8/8/PPPP4/4PPPP/RNBQKBNR w KQ - 0 1");
}

TEST(MakeMove, MoveCastleBlackQueenSide) {
    move_test("r3kbnr/ppp1pppp/2nqb3/3p4/8/PPPP4/4PPPP/RNBQKBNR b KQkq - 0 1",
        encode_move(E8, C8, k, p, 0, 0, 0, 1),
        "2kr1bnr/ppp1pppp/2nqb3/3p4/8/PPPP4/4PPPP/RNBQKBNR w KQ - 0 1");
}

TEST(MakeMove, MoveWhitePromotionQueen) {
    move_test("rnbqkb1r/ppppppPp/5n2/8/8/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1",
        encode_move(G7, G8, P, Q, 0, 0, 0, 0),
        "rnbqkbQr/pppppp1p/5n2/8/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 1");
}

TEST(MakeMove, MoveWhitePromotionRook) {
    move_test("rnbqkb1r/ppppppPp/5n2/8/8/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1",
        encode_move(G7, G8, P, R, 0, 0, 0, 0),
        "rnbqkbRr/pppppp1p/5n2/8/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 1");
}

TEST(MakeMove, MoveWhitePromotionBishop) {
    move_test("rnbqkb1r/ppppppPp/5n2/8/8/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1",
        encode_move(G7, G8, P, B, 0, 0, 0, 0),
        "rnbqkbBr/pppppp1p/5n2/8/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 1");
}

TEST(MakeMove, MoveWhitePromotionKnight) {
    move_test("rnbqkb1r/ppppppPp/5n2/8/8/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1",
        encode_move(G7, G8, P, N, 0, 0, 0, 0),
        "rnbqkbNr/pppppp1p/5n2/8/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 1");
}

TEST(MakeMove, MoveBlackPromotionQueen) {
    move_test("rnbqkbnr/1ppppppp/8/8/8/2NPPP2/PpP3PP/R1BQKBNR b KQkq - 0 1",
        encode_move(B2, B1, p, q, 0, 0, 0, 0),
        "rnbqkbnr/1ppppppp/8/8/8/2NPPP2/P1P3PP/RqBQKBNR w KQkq - 0 1");
}

TEST(MakeMove, MoveBlackPromotionRook) {
    move_test("rnbqkbnr/1ppppppp/8/8/8/2NPPP2/PpP3PP/R1BQKBNR b KQkq - 0 1",
        encode_move(B2, B1, p, r, 0, 0, 0, 0),
        "rnbqkbnr/1ppppppp/8/8/8/2NPPP2/P1P3PP/RrBQKBNR w KQkq - 0 1");
}

TEST(MakeMove, MoveBlackPromotionBishop) {
    move_test("rnbqkbnr/1ppppppp/8/8/8/2NPPP2/PpP3PP/R1BQKBNR b KQkq - 0 1",
        encode_move(B2, B1, p, b, 0, 0, 0, 0),
        "rnbqkbnr/1ppppppp/8/8/8/2NPPP2/P1P3PP/RbBQKBNR w KQkq - 0 1");
}

TEST(MakeMove, MoveBlackPromotionKnight) {
    move_test("rnbqkbnr/1ppppppp/8/8/8/2NPPP2/PpP3PP/R1BQKBNR b KQkq - 0 1",
        encode_move(B2, B1, p, n, 0, 0, 0, 0),
        "rnbqkbnr/1ppppppp/8/8/8/2NPPP2/P1P3PP/RnBQKBNR w KQkq - 0 1");
}

TEST(MakeMove, MoveWhiteCapturePromotionQueen) {
    move_test("rnbqkbnr/ppppppP1/8/8/7p/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1",
        encode_move(G7, H8, P, Q, 1, 0, 0, 0),
        "rnbqkbnQ/pppppp2/8/8/7p/8/PPPPPPP1/RNBQKBNR b KQq - 0 1");
}

TEST(MakeMove, MoveBlackCapturePromotionQueen) {
    move_test("rnbqkbnr/pppppp1p/8/8/8/5R2/PPPPPPPp/RNBQKBN1 b Qkq - 0 1",
        encode_move(H2, G1, p, q, 1, 0, 0, 0),
        "rnbqkbnr/pppppp1p/8/8/8/5R2/PPPPPPP1/RNBQKBq1 w Qkq - 0 1");
}

void unmove_test(const char *start, Move move) {
    Board bd = Board(start);
    IrreversableState state = IrreversableState(bd, move);
    bd.make_move(move);
    unmake_move(bd, move, state);
    Board correct_result = Board(start);
    EXPECT_EQ(bd, correct_result);
    EXPECT_EQ(bd.hash_key, correct_result.zHash());
}

TEST(UnmakeMove, WhitePawnPush) {
    unmove_test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        encode_move(A2, A3, P, p, 0, 0, 0, 0));
}

TEST(UnmakeMove, WhitePawnDoublePush) {
    unmove_test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        encode_move(A2, A4, P, p, 0, 1, 0, 0));
}

TEST(UnmakeMove, WhiteKnight) {
    unmove_test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        encode_move(B1, C3, N, p, 0, 0, 0, 0));
}

TEST(UnmakeMove, BlackPawnPush) {
    unmove_test("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1",
        encode_move(A7, A6, p, p, 0, 0, 0, 0));
}

TEST(UnmakeMove, BlackDoublePawnPush) {
    unmove_test("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1",
        encode_move(A7, A5, p, p, 0, 1, 0, 0));
}

TEST(UnmakeMove, BlackKnight) {
    unmove_test("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1",
        encode_move(B8, A6, n, p, 0, 0, 0, 0));
}

TEST(UnmakeMove, WhiteCastleKingSide) {
    unmove_test("rnbqk2r/pppp1ppp/3b1n2/4p3/8/3BPN2/PPPP1PPP/RNBQK2R w KQkq - 0 1",
        encode_move(E1, G1, K, p, 0, 0, 0, 1));
}

TEST(UnmakeMove, WhiteBishop) {
    unmove_test("rnbqk2r/pppp1ppp/3b1n2/4p3/8/3BPN2/PPPP1PPP/RNBQK2R w KQkq - 0 1",
        encode_move(E3, G6, B, p, 0, 0, 0, 0));
}

TEST(UnmakeMove, WhiteBishopCapture) {
    unmove_test("rnbqk2r/pppp1ppp/3b1n2/4p3/8/3BPN2/PPPP1PPP/RNBQK2R w KQkq - 0 1",
        encode_move(E3, H7, B, p, 1, 0, 0, 0));
}

TEST(UnmakeMove, BlackBishop) {
    unmove_test("rnbqk2r/pppp1ppp/B7/4p3/1b4nP/4PN2/PPPPKPP1/RNBQ3R b kq h3 0 1",
        encode_move(B4, C3, b, p, 0, 0, 0, 0));
}

TEST(UnmakeMove, BlackBishopCapture) {
    unmove_test("rnbqk2r/pppp1ppp/B7/4p3/1b4nP/4PN2/PPPPKPP1/RNBQ3R b kq h3 0 1",
        encode_move(B4, D2, b, p, 1, 0, 0, 0));
}

TEST(UnmakeMove, BlackCastleKingSide) {
    unmove_test("rnbqk2r/pppp1ppp/B7/4p3/1b4nP/4PN2/PPPPKPP1/RNBQ3R b kq h3 0 1",
        encode_move(E8, G8, k, p, 0, 0, 0, 1));
}

TEST(UnmakeMove, BlackPawnCapture) {
    unmove_test("rnbqk2r/pppp1ppp/B7/4p3/1b4nP/4PN2/PPPPKPP1/RNBQ3R b kq h3 0 1",
        encode_move(B7, A6, p, p, 1, 0, 0, 0));
}

TEST(UnmakeMove, WhiteEnpassant) {
    unmove_test("rnbqk2r/pp1p1p1p/B1p5/4p1pP/1b4n1/4PN2/PPPPKPP1/RNBQ3R w kq g6 0 1",
       encode_move(H5, G6, P, p, 1, 0, 1, 0));
}

TEST(UnmakeMove, WhiteIgnoreEnpassant) {
    unmove_test("rnbqk2r/pp1p1p1p/B1p5/4p1pP/1b4n1/4PN2/PPPPKPP1/RNBQ3R w kq - 0 1",
        encode_move(E2, F1, K, p, 0, 0, 0, 0));
}

TEST(UnmakeMove, WhitePromotionQueen) {
    unmove_test("rnbqk2r/pppp1pP1/B7/7p/1b2p1n1/4PN2/PPPPKPP1/RNBQ3R w kq - 0 1",
        encode_move(G7, G8, P, Q, 0, 0, 0, 0));
}

TEST(UnmakeMove, WhitePromotionRook) {
    unmove_test("rnbqk2r/pppp1pP1/B7/7p/1b2p1n1/4PN2/PPPPKPP1/RNBQ3R w kq - 0 1",
        encode_move(G7, G8, P, R, 0, 0, 0, 0));
}

TEST(UnmakeMove, WhiteCapturePromotionQueen) {
    unmove_test("rnbqk2r/pppp1pP1/B7/7p/1b2p1n1/4PN2/PPPPKPP1/RNBQ3R w kq - 0 1",
        encode_move(G7, H8, P, Q, 1, 0, 0, 0));
}

TEST(UnmakeMove, WhiteCapturePromotionRook) {
    unmove_test("rnbqk2r/pppp1pP1/B7/7p/1b2p1n1/4PN2/PPPPKPP1/RNBQ3R w kq - 0 1",
        encode_move(G7, H8, P, R, 1, 0, 0, 0));
}

TEST(UnmakeMove, BlackKnightCapture) {
    unmove_test("rnbqk2r/pppp1ppp/B7/4p3/1b4nP/4PN2/PPPPKPP1/RNBQ3R b kq h3 0 1",
        encode_move(G4, F2, n, p, 1, 0, 0, 0));
}

TEST(UnmakeMove, BlackKnightCapture2) {
    unmove_test("rnbqk2r/pppp1ppp/B7/4p3/1b4nP/4PN2/PPPPKPP1/RNBQ3R b kq - 0 1",
        encode_move(G4, E3, n, p, 1, 0, 0, 0));
}

TEST(UnmakeMove, BlackKnightCheck) {
    unmove_test("rnbqk2r/pppp1ppp/B7/8/1b2p1nP/4PN2/PPPP1PP1/RNBQ1K1R b kq - 0 1",
        encode_move(G4, H2, n, p, 0, 0, 0, 0));
}

TEST(UnmakeMove, BlackKnightCheckCapture) {
    unmove_test("rnbqk2r/pppp1ppp/B7/8/1b2p1nP/4PN2/PPPP1PP1/RNBQ1K1R b kq - 0 1",
        encode_move(G4, E3, n, p, 1, 0, 0, 0));
}

TEST(UnmakeMove, BlackKnightMove2) {
    unmove_test("rnbqk2r/pppp1ppp/B7/8/1b2p1nP/4PN2/PPPP1PP1/RNBQ1K1R b kq - 0 1",
        encode_move(G4, F6, n, p, 0, 0, 0, 0));
}

TEST(UnmakeMove, BlackKnightCheckmate) {
    unmove_test("rnbqr1k1/p1pp1ppp/Bp6/8/1b2p1nP/4PR2/PPPPNPP1/R1B1QKN1 b - - 0 1",
        encode_move(G4, H2, n, p, 0, 0, 0, 0));
}


void unnullmove_test(const char *start) {
    Board bd = Board(start);
    IrreversableState state = IrreversableState(bd, Move(0));
    make_null_move(bd);
    unmake_null_move(bd, state);
    Board correct_result = Board(start);
    EXPECT_EQ(bd, correct_result);
    EXPECT_EQ(bd.hash_key, correct_result.zHash());
}


TEST(UnmakeMove, NullMove) {
    unnullmove_test("rnbqk2r/pppp1ppp/B7/4p3/1b4nP/4PN2/PPPPKPP1/RNBQ3R b kq h3 0 1");
}

TEST(UnmakeMove, NullMoveEnpassant) {
    unnullmove_test("rnbqk2r/pp1p1p1p/B1p5/4p1pP/1b4n1/4PN2/PPPPKPP1/RNBQ3R w kq - 0 1");
}


TEST(EP_IndexCalc, A3) {
    EXPECT_EQ(ep_index_calc(A3), 0);
}

TEST(EP_IndexCalc, H3) {
    EXPECT_EQ(ep_index_calc(H3), 7);
}

TEST(EP_IndexCalc, A6) {
    EXPECT_EQ(ep_index_calc(A6), 8);
}

TEST(EP_IndexCalc, H6) {
    EXPECT_EQ(ep_index_calc(H6), 15);
}





/*
TEST(MoveOrdering, Sorting) {
    const char *many_attacks = "4k3/1pppp3/2rRnbpr/p2PQ1B1/1PP2p1n/1Nq5/PBRPpPPP/1NbK4 b - - 0 1";

    Board board = Board(many_attacks);
    std::vector<Move> move_list;
    generate_moves(board, move_list);
    sort_moves(board, move_list);

    std::vector<std::array<char, 8>> expected_arrays = {
        {E2, D1, p, q, 1, 0, 0, 0},
        {E2, D1, p, b, 1, 0, 0, 0},
        {E2, D1, p, n, 1, 0, 0, 0},
        {E2, D1, p, r, 1, 0, 0, 0},
        {F6, E5, b, p, 1, 0, 0, 0},
        {C3, E5, q, p, 1, 0, 0, 0},
        {C7, D6, p, p, 1, 0, 0, 0},
        {E7, D6, p, p, 1, 0, 0, 0},
        {C6, D6, r, p, 1, 0, 0, 0},
        {C3, C2, q, p, 1, 0, 0, 0},
        {E6, G5, n, p, 1, 0, 0, 0},
        {C1, B2, b, p, 1, 0, 0, 0},
        {F6, G5, b, p, 1, 0, 0, 0},
        {C3, B2, q, p, 1, 0, 0, 0},
        {C3, B3, q, p, 1, 0, 0, 0},
        {A5, B4, p, p, 1, 0, 0, 0},
        {H4, G2, n, p, 1, 0, 0, 0},
        {C1, D2, b, p, 1, 0, 0, 0},
        {C6, C4, r, p, 1, 0, 0, 0},
        {C3, C4, q, p, 1, 0, 0, 0},
        {C3, B4, q, p, 1, 0, 0, 0},
        {C3, D2, q, p, 1, 0, 0, 0},
        {B7, B5, p, p, 0, 1, 0, 0},
        {A5, A4, p, p, 0, 0, 0, 0},
        {F4, F3, p, p, 0, 0, 0, 0},
        {E2, E1, p, r, 0, 0, 0, 0},
        {C3, D3, q, p, 0, 0, 0, 0},
        {C3, E3, q, p, 0, 0, 0, 0},
        {C3, F3, q, p, 0, 0, 0, 0},
        {C3, G3, q, p, 0, 0, 0, 0},
        {C3, H3, q, p, 0, 0, 0, 0},
        {E2, E1, p, n, 0, 0, 0, 0},
        {E2, E1, p, b, 0, 0, 0, 0},
        {C3, D4, q, p, 0, 0, 0, 0},
        {E2, E1, p, q, 0, 0, 0, 0},
        {E8, F7, k, p, 0, 0, 0, 0},
        {E8, D8, k, p, 0, 0, 0, 0},
        {E8, F8, k, p, 0, 0, 0, 0},
        {H6, H8, r, p, 0, 0, 0, 0},
        {H6, H7, r, p, 0, 0, 0, 0},
        {H6, H5, r, p, 0, 0, 0, 0},
        {B7, B6, p, p, 0, 0, 0, 0},
        {C6, B6, r, p, 0, 0, 0, 0},
        {C6, A6, r, p, 0, 0, 0, 0},
        {C6, C5, r, p, 0, 0, 0, 0},
        {H4, F3, n, p, 0, 0, 0, 0},
        {F6, H8, b, p, 0, 0, 0, 0},
        {F6, G7, b, p, 0, 0, 0, 0},
        {H4, F5, n, p, 0, 0, 0, 0},
        {E6, D4, n, p, 0, 0, 0, 0},
        {E6, C5, n, p, 0, 0, 0, 0},
        {E6, G7, n, p, 0, 0, 0, 0},
        {E6, F8, n, p, 0, 0, 0, 0},
        {E6, D8, n, p, 0, 0, 0, 0},
    };

    std::vector<Move> expected;
    for (auto um: expected_arrays) {
        expected.push_back(call_encode(um));
    }

    EXPECT_EQ(move_list, expected);
}
*/

// TEST(FindMate) {

// }

int main(int argc, char **argv) {
    // const char *start_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBN1 w KQkq - 0 1 ";

    // printf("\n");

    // Board b = Board(start_position);

    // int old_eval = evaluate(b);
    // int pesto = pesto_eval(b);

    //printf("old: %d, pesto: %d\n", old_eval, pesto);






    //::testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
    return 0;
}
