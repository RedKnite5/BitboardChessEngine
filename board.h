#pragma once

#include <cstdint>

// FEN dedug positions
inline const char *empty_board = "8/8/8/8/8/8/8/8 w - - 0 1";
inline const char *start_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";
inline const char *tricky_position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
inline const char *killer_position = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
inline const char *cmk_position = "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";


constexpr bool BLACK = false;
constexpr bool WHITE = true;
enum Square {
    A1 = 0, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, No_Square
};

inline const char *square_coords[] = {
    "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
    "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
    "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
    "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
    "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
    "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
    "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
    "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8", "NONE"
};


enum Pieces {
    p, r, n, b, q, k,  // Black pieces
    P, R, N, B, Q, K,  // White pieces
    Last
};

inline const char ascii_pieces[] = "prnbqkPRNBQK";
inline const char *unicode_pieces[12] = {"♙", "♖", "♘", "♗", "♕", "♔",
                                  "♟", "♜", "♞", "♝", "♛", "♚"};





void print_move(int move);

void print_move_list(std::vector<int> &move_list);

class Board {
  public:
    std::array<uint64_t, 12> bitboards;
    std::array<uint64_t, 2> coloredPieces;
    uint64_t allPieces = 0;
    Square enPassantSquare = No_Square;
    unsigned short move;
    unsigned char halfMoveClock;  //  draw claimable at 100, forced at 150
    unsigned char castleFlags;
    // turn: true for white, false for black
    bool turn;

    Board();

    // Constructor to initialize from a FEN string
    Board(const char *str);

};


void printBitBoard_stream(uint64_t bitBoard, std::ostream& out);
void printBitBoard(uint64_t bitBoard);

void printBoard_stream(Board &board, std::ostream& out);
void printBoard(Board &board);


constexpr inline uint64_t get_bishop_attacks(unsigned char square, uint64_t occupancy);
constexpr inline uint64_t get_rook_attacks(unsigned char square, uint64_t occupancy);
constexpr inline uint64_t get_queen_attacks(unsigned char square, uint64_t occupancy);
constexpr inline uint64_t get_quiet_pawn_moves(unsigned char square, uint64_t occupancy, int side);

inline bool is_square_attacked(Board &board, unsigned char square, bool side);


//void generate_pawn_moves_white(Board &board, std::vector<int> &move_list);

inline bool is_king_exposed(Board &bd);

void generate_moves(Board &board, std::vector<int> &move_list);

bool make_move(Board &board, int move);


void printAllAttackedSquares(Board &board, bool side);





