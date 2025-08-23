#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <random>
#include <type_traits>
#include <vector>
#include <array>
#include <cassert>
#include <algorithm>

using U64 = uint64_t;

#include "board.h"


/*
   not A file

8  . 1 1 1 1 1 1 1
7  . 1 1 1 1 1 1 1
6  . 1 1 1 1 1 1 1
5  . 1 1 1 1 1 1 1
4  . 1 1 1 1 1 1 1
3  . 1 1 1 1 1 1 1
2  . 1 1 1 1 1 1 1
1  . 1 1 1 1 1 1 1

   A B C D E F G H
*/
constexpr U64 NOT_A_FILE = 18374403900871474942ULL;

/*
   not H file
8  1 1 1 1 1 1 1 .
7  1 1 1 1 1 1 1 .
6  1 1 1 1 1 1 1 .
5  1 1 1 1 1 1 1 .
4  1 1 1 1 1 1 1 .
3  1 1 1 1 1 1 1 .
2  1 1 1 1 1 1 1 .
1  1 1 1 1 1 1 1 .

   A B C D E F G H
*/
constexpr U64 NOT_H_FILE = 9187201950435737471ULL;

/*
   not AB files
8  . . 1 1 1 1 1 1
7  . . 1 1 1 1 1 1
6  . . 1 1 1 1 1 1
5  . . 1 1 1 1 1 1
4  . . 1 1 1 1 1 1
3  . . 1 1 1 1 1 1
2  . . 1 1 1 1 1 1
1  . . 1 1 1 1 1 1

   A B C D E F G H
*/
constexpr U64 NOT_AB_FILES = 18229723555195321596ULL;

/*
   not GH files
8  1 1 1 1 1 1 . .
7  1 1 1 1 1 1 . .
6  1 1 1 1 1 1 . .
5  1 1 1 1 1 1 . .
4  1 1 1 1 1 1 . .
3  1 1 1 1 1 1 . .
2  1 1 1 1 1 1 . .
1  1 1 1 1 1 1 . .

   A B C D E F G H
*/
constexpr U64 NOT_GH_FILES = 4557430888798830399ULL;


// Bishop relivant occupancy bit counts
constexpr int bishop_relevant_occupancy_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};
// Rook relivant occupancy bit counts
constexpr int rook_relevant_occupancy_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};


constexpr U64 bishopMagicNumbers[64] = {
    563504205534720,
    45618738787025920,
    2308096063191515456,
    1731707328970915840,
    9306283732893904,
    44118105427969,
    1153492202715349504,
    31595600497148160,
    17872449519648,
    2306124760211260544,
    1154060603518814208,
    655348581369648131,
    619026120179968,
    36028870302925056,
    2323858524490437441,
    72420501881921540,
    1153071055385592337,
    144257128157192224,
    2918474399831818400,
    2251804922413125,
    20407571493947392,
    55732055061826888,
    144423069619324040,
    154811237250924680,
    1441723768841011233,
    652124761694421508,
    1224987993589809744,
    1134696006164800,
    1157706648215633920,
    36591781365710912,
    2315413708211228804,
    88235825045512,
    19709918454112256,
    4857676733879041,
    577591120586899586,
    189120295995648,
    5067116516605984,
    586365157475911696,
    149463865745232960,
    72066537241973760,
    38563172399187972,
    13242474696904,
    432521693656384000,
    6919781102471086208,
    144434364279705634,
    2596431863577849984,
    145804330019061921,
    779038918443012,
    72198422257584130,
    4506116746903816,
    72057611237196291,
    866107334211077121,
    9710955752095872,
    1167012931561326608,
    6936118264581881988,
    4909816775243243524,
    14293787607168,
    468376560337190976,
    2823546163255361,
    738665243118707722,
    4688407878279898145,
    40532535163494410,
    17596752134161,
    85291385252758272
};

constexpr U64 rookMagicNumbers[64] = {
    612509890289811457,
    3461019131139072137,
    4629982029486755840,
    1171015068221980674,
    720580411540906241,
    72130161872472832,
    1197957784357306880,
    144115326622402308,
    2341889536000344640,
    433823445298450440,
    599049669208711176,
    17600776388624,
    18058379109072936,
    22561982981079170,
    25121852133214226,
    2305958458471489664,
    18157884781101136,
    1153484510902894785,
    3607383438964824064,
    22520265882206257,
    9009501373990912,
    4613938934951510528,
    1161933170732435472,
    297240599166226688,
    2341873047479287873,
    5260230890756244480,
    8796630418432,
    1164324007659602048,
    18581802478213168,
    720576490673078784,
    2311762818606598672,
    290781243673411648,
    756604823300210816,
    8719954080065192960,
    4623191308236752000,
    4503883330158880,
    4611756404892502272,
    2323302966235145,
    10191407704736256,
    217017207597826112,
    18017147289866240,
    17593404560384,
    63191836646441216,
    4611703920396730372,
    434599641783812098,
    580966551289626629,
    4503883095244929,
    4612002683147157515,
    2883448490564978240,
    4630263439905275973,
    72061992388526096,
    1157566116634567681,
    288379909884432388,
    4611722388210516608,
    253332160555009032,
    9075265963360,
    36031271333298433,
    612509375432573058,
    616997616906952769,
    564067752747010,
    72059827421184769,
    36353211891842,
    81206703450098756,
    576461044378009634
};

constexpr inline int count_bits(unsigned long long x) {
    return __builtin_popcountll(x);
}

constexpr inline int get_lsb_index(unsigned long long x) {
    return __builtin_ctzll(x);
}

constexpr inline int get_lsb_index_safe(U64 bitBoard) {
    int is_zero = bitBoard == 0;
    int safe_ctz = get_lsb_index(bitBoard | is_zero);
    return safe_ctz - is_zero;
}

constexpr U64 generatePawnAttackMask(U64 square, bool color) {
    U64 mask = 0;
    if (color) { // White pawns
        mask |= (square << 9) & NOT_A_FILE; // Capture right
        mask |= (square << 7) & NOT_H_FILE; // Capture left
    } else {     // Black pawns
        mask |= (square >> 9) & NOT_H_FILE; // Capture left (white's perspective)
        mask |= (square >> 7) & NOT_A_FILE; // Capture right
    }
    return mask;
}
constexpr auto generatePawnAttackMasks() {
    std::array<std::array<U64, 64>, 2> pawnAttackMasks = {};
    for (int i = 0; i < 64; ++i) {
        pawnAttackMasks[WHITE][i] = generatePawnAttackMask(1ULL << i, WHITE);
        pawnAttackMasks[BLACK][i] = generatePawnAttackMask(1ULL << i, BLACK);
    }
    return pawnAttackMasks;
}

constexpr auto generatePawnPushMasks() {
    std::array<std::array<U64, 56>, 2> pawnPushMasks = {};
    for (unsigned int i=0; i<56; ++i) {
        pawnPushMasks[WHITE][i] = 1ULL << (i + 8);
        if (i >= 8) {
            pawnPushMasks[BLACK][i] = 1ULL << (i - 8);
        }
        if (i >= 8 && i <= 15) {
            pawnPushMasks[WHITE][i] |= 1ULL << (i + 16);
        }
        if (i >= 48 && i <= 55) {
            pawnPushMasks[BLACK][i] |= 1ULL << (i - 16);
        }
    }
    return pawnPushMasks;
}

constexpr U64 generateKnightAttackMask(U64 square) {
    U64 mask = 0;
    mask |= (square << 15) & NOT_H_FILE; // Up-Left
    mask |= (square << 17) & NOT_A_FILE; // Up-Right
    mask |= (square >> 15) & NOT_A_FILE; // Down-Right
    mask |= (square >> 17) & NOT_H_FILE; // Down-Left
    mask |= (square << 6) & NOT_GH_FILES;  // Left-Up
    mask |= (square << 10) & NOT_AB_FILES; // Right-Up
    mask |= (square >> 6) & NOT_AB_FILES;  // Right-Down
    mask |= (square >> 10) & NOT_GH_FILES; // Left-Down
    return mask;
}
constexpr std::array<U64, 64> generateKnightAttackMasks() {
    std::array<U64, 64> knightAttackMasks = {};
    for (int i = 0; i < 64; ++i) {
        knightAttackMasks[i] = generateKnightAttackMask(1ULL << i);
    }
    return knightAttackMasks;
}

constexpr U64 generateKingAttackMask(U64 square) {
    U64 mask = 0;
    mask |= (square << 1) & NOT_A_FILE; // Right
    mask |= (square >> 1) & NOT_H_FILE; // Left
    mask |= (square << 8);               // Up
    mask |= (square >> 8);               // Down
    mask |= (square << 9) & NOT_A_FILE;  // Up-Right
    mask |= (square << 7) & NOT_H_FILE;  // Up-Left
    mask |= (square >> 9) & NOT_H_FILE;  // Down-Left
    mask |= (square >> 7) & NOT_A_FILE;  // Down-Right
    return mask;
}
constexpr std::array<U64, 64> generateKingAttackMasks() {
    std::array<U64, 64> kingAttackMasks = {};
    for (int i = 0; i < 64; ++i) {
        kingAttackMasks[i] = generateKingAttackMask(1ULL << i);
    }
    return kingAttackMasks;
}

struct AttackTables {
    std::array<U64, 64> bishopPsudoAttackMasks;
    std::array<U64, 64> rookPsudoAttackMasks;
    std::array<std::array<U64, 512>, 64> bishopAttackMasks;
    std::array<std::array<U64, 4096>, 64> rookAttackMasks;
};

constexpr U64 generateBishopRelevantOccupancyMask(unsigned char square) {
    U64 mask = 0;

    int tr = square / 8;
    int tf = square % 8;

    for (int i = 1; i < 8; ++i) {
        if (tr + i < 7 && tf + i < 7) {
            // Up-Right
            mask |= (1ULL << ((tr + i) * 8 + (tf + i)));
        }
        if (tr - i >= 1 && tf - i >= 1) {
            // Down-Left
            mask |= (1ULL << ((tr - i) * 8 + (tf - i)));
        }
        if (tr + i < 7 && tf - i >= 1) {
            // Up-Left
            mask |= (1ULL << ((tr + i) * 8 + (tf - i)));
        }
        if (tr - i >= 1 && tf + i < 7) {
            // Down-Right
            mask |= (1ULL << ((tr - i) * 8 + (tf + i)));
        }
    }
    return mask;
}

constexpr U64 generateBishopAttackMask(unsigned char square, U64 occupied) {
    U64 mask = 0;

    int tr = square / 8;
    int tf = square % 8;

    for (int i = tr, j = tf; i < 8 && j < 8; ++i, ++j) {
        // Up-Right
        mask |= 1ULL << (i * 8 + j);
        if (occupied & (1ULL << (i * 8 + j))) {
            break; // Stop if occupied
        }
    }
    for (int i = tr, j = tf; i >= 0 && j >= 0; --i, --j) {
        // Down-Left
        mask |= 1ULL << (i * 8 + j);
        if (occupied & (1ULL << (i * 8 + j))) {
            break;
        }
    }
    for (int i = tr, j = tf; i < 8 && j >= 0; ++i, --j) {
        // Up-Left
        mask |= 1ULL << (i * 8 + j);
        if (occupied & (1ULL << (i * 8 + j))) {
            break;
        }
    }
    for (int i = tr, j = tf; i >= 0 && j < 8; --i, ++j) {
        // Down-Right
        mask |= 1ULL << (i * 8 + j);
        if (occupied & (1ULL << (i * 8 + j))) {
            break;
        }
    }
    // Remove the square itself from the mask
    mask ^= 1ULL << square;

    return mask;
}

constexpr U64 generateRookRelevantOccupancyMask(unsigned char square) {
    U64 mask = 0;

    int tr = square / 8;
    int tf = square % 8;

    for (int i = 1; i < 7; ++i) {
        if (i != tr) {
            // Vertical
            mask |= (1ULL << (i * 8 + tf));
        }
        if (i != tf) {
            // Horizontal
            mask |= (1ULL << (tr * 8 + i));
        }
    }
    return mask;
}

constexpr U64 generateRookAttackMask(unsigned char square, U64 occupied) {
    U64 mask = 0;

    int tr = square / 8;
    int tf = square % 8;

    for (int i = tr+1; i < 8; ++i) {
        // Vertical Up
        mask |= (1ULL << (i * 8 + tf));
        if (occupied & (1ULL << (i * 8 + tf))) {
            break; // Stop if occupied
        }
    }
    for (int i = tr-1; i >= 0; --i) {
        // Vertical Down
        mask |= (1ULL << (i * 8 + tf));
        if (occupied & (1ULL << (i * 8 + tf))) {
            break;
        }
    }
    for (int i = tf+1; i < 8; ++i) {
        // Horizontal Right
        mask |= (1ULL << (tr * 8 + i));
        if (occupied & (1ULL << (tr * 8 + i))) {
            break;
        }
    }
    for (int i = tf-1; i >= 0; --i) {
        // Horizontal Left
        mask |= (1ULL << (tr * 8 + i));
        if (occupied & (1ULL << (tr * 8 + i))) {
            break;
        }
    }

    return mask;
}

constexpr U64 setOccupancy(int index, int bits_in_mask, U64 mask) {
    // index goes up to 2^bits_in_mask
    U64 occupancy = 0;
    for (int i = 0; i < bits_in_mask; ++i) {
        int square = get_lsb_index_safe(mask);
        mask &= mask - 1;
        if (index & (1 << i)) {
            occupancy |= (1ULL << square);
        }
    }
    return occupancy;
}

constexpr AttackTables generateSliderAttackMasks() {
    AttackTables tables = {};
    for (int square=0; square < 64; ++square) {

        tables.bishopPsudoAttackMasks[square] = generateBishopRelevantOccupancyMask(square);
        tables.rookPsudoAttackMasks[square] = generateRookRelevantOccupancyMask(square);

        int bishop_rel_occ_count = bishop_relevant_occupancy_bits[square];
        int bishop_occupancy_indeces = 1ULL << bishop_rel_occ_count;

        for (int i=0; i < bishop_occupancy_indeces; ++i) {
            U64 occupancy = setOccupancy(i, bishop_rel_occ_count, tables.bishopPsudoAttackMasks[square]);
            int magic_index = (occupancy * bishopMagicNumbers[square]) >> (64 - 9);
            tables.bishopAttackMasks[square][magic_index] = generateBishopAttackMask(square, occupancy);
        }

        int rook_rel_occ_count = rook_relevant_occupancy_bits[square];
        int rook_occupancy_indeces = 1ULL << rook_relevant_occupancy_bits[square];

        for (int i=0; i < rook_occupancy_indeces; ++i) {
            U64 occupancy = setOccupancy(i, rook_rel_occ_count, tables.rookPsudoAttackMasks[square]);
            int magic_index = (occupancy * rookMagicNumbers[square]) >> (64 - 12);
            tables.rookAttackMasks[square][magic_index] = generateRookAttackMask(square, occupancy);
        }
    }
    return tables;
}


//*
constexpr AttackTables attackTables = generateSliderAttackMasks();
constexpr auto &bishopPsudoAttackMasks = attackTables.bishopPsudoAttackMasks;  // [square]
constexpr auto &rookPsudoAttackMasks = attackTables.rookPsudoAttackMasks;      // [square]
constexpr auto &bishopAttackMasks = attackTables.bishopAttackMasks;   // [square][occupancy variation]
constexpr auto &rookAttackMasks = attackTables.rookAttackMasks;       // [square][occupancy variation]

constexpr auto pawnAttackMasks = generatePawnAttackMasks();
constexpr auto pawnPushMasks = generatePawnPushMasks();
constexpr std::array<U64, 64> knightAttackMasks = generateKnightAttackMasks();
constexpr std::array<U64, 64> kingAttackMasks = generateKingAttackMasks();


/*
static U64 magic_number_candidate() {
    U64 n1 = random() | (random() << 32);
    U64 n2 = random() | (random() << 32);
    U64 n3 = random() | (random() << 32);
    return n1 & n2 & n3;
}
*/


 //  Castle_wk | Castle_wq | Castle_bk | Castle_bq;
constexpr unsigned char Castle_none = 0;
constexpr unsigned char Castle_wk = 1 << 3;
constexpr unsigned char Castle_wq = 1 << 2;
constexpr unsigned char Castle_bk = 1 << 1;
constexpr unsigned char Castle_bq = 1 << 0;

constexpr U64 castle_bq_mask = 14ULL << 56;
constexpr U64 castle_bk_mask = 3ULL << 61;
constexpr U64 castle_wq_mask = 14;
constexpr U64 castle_wk_mask = 3 << 5;


constexpr std::array<char, 64> castling_rights = {
    11, 15, 15, 15,  3, 15, 15,  7,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    14, 15, 15, 15, 12, 15, 15, 13
};


constexpr inline int piece_select(Pieces piece, bool side) {
    return piece + 6*side;
}

constexpr int TARGET_SHIFT = 6;
constexpr int PIECE_SHIFT = 12;
constexpr int PROMOTION_SHIFT = 16;
constexpr int CAPTURE_SHIFT = 20;
constexpr int DOUBLE_SHIFT = 21;
constexpr int ENPASSANT_SHIFT = 22;
constexpr int CASTLE_SHIFT = 23;

/*
Moves
0000 0000 0000 0000 0011 1111 Source square
0000 0000 0000 1111 1100 0000 Target square
0000 0000 1111 0000 0000 0000 Piece
0000 1111 0000 0000 0000 0000 Promotion Piece
0001 0000 0000 0000 0000 0000 Capture flag
0010 0000 0000 0000 0000 0000 Double push flag
0100 0000 0000 0000 0000 0000 Enpassant flag
1000 0000 0000 0000 0000 0000 Castle flag
*/
constexpr inline int encode_move(int source,
                int target,
                int piece,
                int promotion,
                int capture,
                int double_push,
                int enpassant,
                int castle) {
    return source
            | (target << 6)
            | (piece << 12)
            | (promotion << 16)
            | (capture << 20)
            | (double_push << 21)
            | (enpassant << 22)
            | (castle << 23);
}

constexpr inline int get_move_source(int move) {
    const int source_mask = 0x3F;
    return move & source_mask;
}

constexpr inline int get_move_target(int move) {
    const int target_mask = 0xFC0;
    return (move & target_mask) >> TARGET_SHIFT;
}

constexpr inline int get_move_piece(int move) {
    const int piece_mask = 0xF000;
    return (move & piece_mask) >> PIECE_SHIFT;
}

constexpr inline int get_promotion(int move) {
    const int promotion_mask = 0xF0000;
    return (move & promotion_mask) >> PROMOTION_SHIFT;
}

constexpr int CAPTURE_FLAG = 0x100000;
constexpr inline int get_capture_flag(int move) {
    return move & CAPTURE_FLAG;
}
constexpr int DOUBLE_PUSH_FLAG = 0x200000;
constexpr inline int get_double_push_flag(int move) {
    return move & DOUBLE_PUSH_FLAG;
}
constexpr int ENPASSANT_FLAG = 0x400000;
constexpr inline int get_enpassant_flag(int move) {
    return move & ENPASSANT_FLAG;
}
constexpr int CASTLE_FLAG = 0x800000;
constexpr inline int get_castle_flag(int move) {
    return move & CASTLE_FLAG;
}

void print_move(int move) {
    printf("%c%s%s\n",
        ascii_pieces[get_move_piece(move)],
        square_coords[get_move_source(move)],
        square_coords[get_move_target(move)]
    );
}

void print_move_list(std::vector<int> &move_list) {
    printf("Move List:\n{");
    for (unsigned int i=0; i < move_list.size(); i++) {
        printf("%d, ", move_list[i]);
    }
    printf("}\n");

    for (unsigned int i=0; i < move_list.size(); i++) {
        int move = move_list[i];
        printf("%c%s%s Promotion: %c Capture: %d Double Push: %d Enpassant: %d Castle: %d\n",
            ascii_pieces[get_move_piece(move)],
            square_coords[get_move_source(move)],
            square_coords[get_move_target(move)],
            ascii_pieces[get_promotion(move)],
            get_capture_flag(move) >> CAPTURE_SHIFT,
            get_double_push_flag(move) >> DOUBLE_SHIFT,
            get_enpassant_flag(move) >> ENPASSANT_SHIFT,
            get_castle_flag(move) >> CASTLE_SHIFT
        );
    }
}



Board::Board() {
    move = 0;
    turn = WHITE;
    castleFlags = Castle_wk | Castle_wq | Castle_bk | Castle_bq;
    halfMoveClock = 0;  // 50 move rule
    coloredPieces = {{0xFFFFULL << 48, 0xFFFFULL}};
    U64 whitePawns = 0xFF << 8;
    U64 whiteRooks = 1 | (1 << 7);
    U64 whiteKnights = (1 << 1) | (1 << 6);
    U64 whiteBishops = (1 << 2) | (1 << 5);
    U64 whiteQueens = 1 << 3;
    U64 whiteKing = 1 << 4;
    U64 blackPawns = 0xFFULL << 48;
    U64 blackRooks = (1ULL << 56) | (1ULL << 63);
    U64 blackKnights = (1ULL << 57) | (1ULL << 62);
    U64 blackBishops = (1ULL << 58) | (1ULL << 61);
    U64 blackQueens = 1ULL << 59;
    U64 blackKing = 1ULL << 60;

    bitboards = {
        blackPawns, blackRooks, blackKnights, blackBishops, blackQueens, blackKing,
        whitePawns, whiteRooks, whiteKnights, whiteBishops, whiteQueens, whiteKing
    };

    coloredPieces[BLACK] = blackRooks | blackKnights | blackBishops | blackQueens | blackKing | blackPawns;
    coloredPieces[WHITE] = whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing | whitePawns;
    allPieces = coloredPieces[BLACK] | coloredPieces[WHITE];
}

// Constructor to initialize from a FEN string
Board::Board(const char *str) {
    bitboards = {0};
    int i = 0;
    int square = 56;
    bool piecePlacement = true;
    coloredPieces = {0};
    allPieces = 0;
    while (piecePlacement) {
        char c = str[i];
        switch (c) {
            case 'r':
                bitboards[r] |= 1ULL << square;
                coloredPieces[BLACK] |= 1ULL << square;
                break;
            case 'n':
                bitboards[n] |= 1ULL << square;
                coloredPieces[BLACK] |= 1ULL << square;
                break;
            case 'b':
                bitboards[b] |= 1ULL << square;
                coloredPieces[BLACK] |= 1ULL << square;
                break;
            case 'q':
                bitboards[q] |= 1ULL << square;
                coloredPieces[BLACK] |= 1ULL << square;
                break;
            case 'k':
                bitboards[k] |= 1ULL << square;
                coloredPieces[BLACK] |= 1ULL << square;
                break;
            case 'p':
                bitboards[p] |= 1ULL << square;
                coloredPieces[BLACK] |= 1ULL << square;
                break;
            case 'R':
                bitboards[R] |= 1ULL << square;
                coloredPieces[WHITE] |= 1ULL << square;
                break;
            case 'N':
                bitboards[N] |= 1ULL << square;
                coloredPieces[WHITE] |= 1ULL << square;
                break;
            case 'B':
                bitboards[B] |= 1ULL << square;
                coloredPieces[WHITE] |= 1ULL << square;
                break;
            case 'Q':
                bitboards[Q] |= 1ULL << square;
                coloredPieces[WHITE] |= 1ULL << square;
                break;
            case 'K':
                bitboards[K] |= 1ULL << square;
                coloredPieces[WHITE] |= 1ULL << square;
                break;
            case 'P':
                bitboards[P] |= 1ULL << square;
                coloredPieces[WHITE] |= 1ULL << square;
                break;
            case '/':
                square -= 17;
                break;
            case ' ':
                //break out of the loop when we reach the end of the piece placement
                piecePlacement = false;
                break;
            default:
                if (c >= '1' && c <= '8') {
                    square += (c - '0') - 1; // Move square back by the number of empty squares
                } else {
                    std::cerr << "Invalid character in FEN string piece placement: " << c << std::endl;
                    exit(EXIT_FAILURE);
                }
        }
        i++;
        square++;
    }

    if (str[i] == 'w') {
        turn = 1; // White to move
    } else if (str[i] == 'b') {
        turn = 0; // Black to move
    } else {
        std::cerr << "Invalid turn in FEN string turn: " << str[i] << std::endl;
        exit(EXIT_FAILURE);
    }

    i++;
    if (str[i] != ' ') {
        std::cerr << "Invalid character after turn in FEN string: " << str[i] << std::endl;
        exit(EXIT_FAILURE);
    }
    i++;

    castleFlags = Castle_none;
    bool castling = true;
    while (castling) {
        char c = str[i];
        switch (c) {
            case 'K':
                castleFlags |= Castle_wk;
                break;
            case 'Q':
                castleFlags |= Castle_wq;
                break;
            case 'k':
                castleFlags |= Castle_bk;
                break;
            case 'q':
                castleFlags |= Castle_bq;
                break;
            case '-':
                i++;
                castling = false;
                break;
            case ' ':
                castling = false;
                break;
            default:
                std::cerr << "Invalid character in castling rights: " << c << std::endl;
                exit(EXIT_FAILURE);
        }
        i++;
    }

    if (str[i] == '-') {
        enPassantSquare = No_Square; // No en passant square
        i++;
    } else if (str[i] >= 'a' && str[i] <= 'h' && str[i + 1] >= '1' && str[i + 1] <= '8') {
        enPassantSquare = static_cast<Square>((str[i + 1] - '1') * 8 + (str[i] - 'a'));
        if (str[i+1] == '\0') {
            std::cerr << "Invalid en passant square in FEN string: " << str[i] << str[i + 1] << std::endl;
            exit(EXIT_FAILURE);
        }
        i += 2; // Move past the en passant square
    } else {
        std::cerr << "Invalid en passant square in FEN string at:" << i << " str[i] ='" << str[i] << "'str[i+1] ='" << str[i + 1] << '\'' << std::endl;
        exit(EXIT_FAILURE);
    }

    if (str[i] != ' ') {
        std::cerr << "Invalid character after en passant square in FEN string: " << str[i] << std::endl;
        exit(EXIT_FAILURE);
    }
    i++;

    // Half-move clock
    halfMoveClock = 0;
    while (str[i] >= '0' && str[i] <= '9') {
        halfMoveClock = halfMoveClock * 10 + (str[i] - '0');
        i++;
    }
    
    if (str[i] != ' ') {
        std::cerr << "Invalid character after half-move clock in FEN string:" << std::endl;
        std::cerr << "str[i] = '" << str[i] << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    i++;

    // Full-move number
    move = 0;
    while (str[i] >= '0' && str[i] <= '9') {
        move = move * 10 + (str[i] - '0');
        i++;
    }
    // Update allPieces
    allPieces = coloredPieces[WHITE] | coloredPieces[BLACK];
}

void printBitBoard_stream(U64 bitBoard, std::ostream& out) {
    for (int row = 7; row >= 0; --row) {
        out << row + 1 << "  ";
        for (int col = 0; col < 8; ++col) {
            int index = row * 8 + col;
            if (bitBoard & (1ULL << index)) {
                out << "1 ";
            } else {
                out << ". ";
            }
        }
        out << std::endl;
    }
    out << "\n   A B C D E F G H" << std::endl;
    out << bitBoard << std::endl;
}

void printBitBoard(U64 bitBoard) {
    printBitBoard_stream(bitBoard, std::cout);
}


void printBoard_stream(const Board &board, std::ostream& out) {
    for (int row = 7; row >= 0; --row) {
        out << row + 1 << "  ";
        for (int col = 0; col < 8; ++col) {
            int index = row * 8 + col;
            for (int piece = 0; piece != Last; ++piece) {
                if (board.bitboards[piece] & (1ULL << index)) {
                    out << unicode_pieces[piece] << " ";
                    break;
                }
                if (piece == Last-1) { // If no piece found, print empty square
                    out << ". ";
                }
            }
        }
        out << "\n";
    }
    out << "\n   A B C D E F G H\n";
    out << "Move: " << board.move << "\n";
    out << "Castle Flags: ";
    const char castling_key[] = "KQkq";
    for (int i = 0; i < 4; ++i) {
        if ((board.castleFlags & 1 << (3-i)) != Castle_none) {
            out << castling_key[i];
        } else {
            out << "-";
        }
    }
    out << "\n";
    out << "En Passant Square: " << square_coords[board.enPassantSquare] << "\n";
    out << "Half Move Clock: " << board.halfMoveClock << "\n";
    out << (board.turn ? "White to move" : "Black to move") << std::endl;
}

void printBoard(const Board &board) {
    printBoard_stream(board, std::cout);
}

/* Magic Numbers
U64 magic_number(int square, int relivant_bits, bool bishop) {
    U64 occupancies[4096];
    U64 attacks[4096];
    U64 used_attacks[4096];

    U64 mask;
    if (bishop) {
        mask = generateBishopRelevantOccupancyMask(square);
    } else {
        mask = generateRookRelevantOccupancyMask(square);
    }

    int occupancy_ideces = 1 << relivant_bits;

    for (int i = 0; i < occupancy_ideces; ++i) {
        U64 occupancy = setOccupancy(i, relivant_bits, mask);
        occupancies[i] = occupancy;

        if (bishop) {
            attacks[i] = generateBishopAttackMask(square, occupancy);
        } else {
            attacks[i] = generateRookAttackMask(square, occupancy);
        }
    }

    int bit_shift = bishop ? 9 : 12;

    while (true) {
        U64 magic = magic_number_candidate();
        if (magic == 0) continue; // Magic number cannot be zero

        memset(used_attacks, 0, sizeof(used_attacks));

        //std::cout << "\n\nmagic: " << magic;

        bool valid = true;
        for (int i = 0; i < occupancy_ideces; ++i) {
            int index = (occupancies[i] * magic) >> (64 - bit_shift);
            if (used_attacks[index] != 0 && used_attacks[index] != attacks[i]) {
                valid = false;
                break;
            }
            used_attacks[index] = attacks[i];
            //std::cout << "occupancy:\n";
            //printBitBoard(occupancies[i]);
            //std::cout << "attacks:\n";
            //printBitBoard(attacks[i]);
            //std::cout << "index: " << index << '\n' << std::endl;
        }

        if (valid) {
            return magic;
        }
        //printf("Invalid magic number: %llu\n", magic);
    }
}

void init_magic_numbers() {
    // Generate magic numbers for bishops
    for (int square = 0; square < 64; ++square) {
        int bits = bishop_relevant_occupancy_bits[square];
        U64 magic = magic_number(square, bits, true);
        std::cout << magic << ", " << std::endl;
    }

    std::cout << "\n\n";

    // Generate magic numbers for rooks
    for (int square = 0; square < 64; ++square) {
        int bits = rook_relevant_occupancy_bits[square];
        U64 magic = magic_number(square, bits, false);
        std::cout << magic << ", " << std::endl;
    }
}
*/


inline U64 get_bishop_attacks(unsigned char square, U64 occupancy) {
    occupancy &= bishopPsudoAttackMasks[square];
    occupancy *= bishopMagicNumbers[square];
    occupancy >>= 64 - 9;
    return bishopAttackMasks[square][occupancy];
}

inline U64 get_rook_attacks(unsigned char square, U64 occupancy) {
    occupancy &= rookPsudoAttackMasks[square];
    occupancy *= rookMagicNumbers[square];
    occupancy >>= 64 - 12;
    return rookAttackMasks[square][occupancy];
}

inline U64 get_queen_attacks(unsigned char square, U64 occupancy) {
    return get_bishop_attacks(square, occupancy) | get_rook_attacks(square, occupancy);
}

inline U64 get_quiet_pawn_moves(unsigned char square, U64 occupancy, int side) {
    int shift = side * 16 - 8;

    // 0 if next pawn square, (square + 8), is blocked
    // 1 if it is free
    U64 next_square_blocked = 1ULL - (1ULL & (occupancy >> (square + shift)));
    return next_square_blocked * (pawnPushMasks[side][square] & ~occupancy);
}

// is square attacked by side
inline bool is_square_attacked(const Board &board, unsigned char square, bool side) {
    // if a black pawn was on square then the two spaces it attacks are where
    // that square could be attacked from by white pawns
    U64 attacked = pawnAttackMasks[!side][square] & board.bitboards[piece_select(p, side)];
    attacked |= knightAttackMasks[square] & board.bitboards[piece_select(n, side)];
    if (attacked) {
        return attacked;
    }
    attacked = get_bishop_attacks(square, board.allPieces);
    attacked &= (board.bitboards[piece_select(b, side)] | board.bitboards[piece_select(q, side)]);
    if (attacked) {
        return attacked;
    }
    attacked = get_rook_attacks(square, board.allPieces);
    attacked &= (board.bitboards[piece_select(r, side)] | board.bitboards[piece_select(q, side)]);
    attacked |= kingAttackMasks[square] & board.bitboards[piece_select(k, side)];

    return attacked;
}


void generate_pawn_moves_white(const Board &board, std::vector<int> &move_list) {
    U64 p_bb = board.bitboards[P];
    int source_sq, target_sq, cur_move;
    while (p_bb) {
        source_sq = get_lsb_index(p_bb);
        bool not_promoting = source_sq < A7;

        U64 attacks = pawnAttackMasks[WHITE][source_sq];
        attacks &= board.coloredPieces[BLACK] | (1ULL << board.enPassantSquare);
        int move = source_sq | (P << PIECE_SHIFT);

        U64 pushes = get_quiet_pawn_moves(source_sq, board.allPieces, WHITE);
        while (pushes) {
            target_sq = get_lsb_index(pushes);
            cur_move = move | (target_sq << 6);

            if (not_promoting) {
                //                      Double Pawn FLag
                cur_move |= ((target_sq == source_sq + 16) << DOUBLE_SHIFT);
                move_list.push_back(cur_move);
            } else {
                //                               Promotion
                move_list.push_back(cur_move | (Q << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (B << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (N << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (R << PROMOTION_SHIFT));
            }

            pushes ^= (1ULL << target_sq);
        }

        move |= CAPTURE_FLAG;

        while (attacks) {
            target_sq = get_lsb_index(attacks);
            cur_move = move | (target_sq << TARGET_SHIFT);

            if (not_promoting) {
                //                               Enpassant FLag
                int enpassant_flag = (board.enPassantSquare == target_sq) << ENPASSANT_SHIFT;
                move_list.push_back(cur_move | enpassant_flag);
            } else {
                //                               Promotion
                move_list.push_back(cur_move | (Q << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (B << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (N << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (R << PROMOTION_SHIFT));
            }

            attacks ^= 1ULL << target_sq;
        }

        p_bb ^= 1ULL << source_sq;
    }
}

void generate_pawn_moves_black(const Board &board, std::vector<int> &move_list) {
    U64 p_bb = board.bitboards[p];
    int source_sq, target_sq, cur_move;
    while (p_bb) {
        source_sq = get_lsb_index(p_bb);
        bool not_promoting = source_sq > H2;

        U64 attacks = pawnAttackMasks[BLACK][source_sq];
        attacks &= board.coloredPieces[WHITE] | (1ULL << board.enPassantSquare);
        int move = source_sq | (p << PIECE_SHIFT);

        U64 pushes = get_quiet_pawn_moves(source_sq, board.allPieces, BLACK);
        while (pushes) {
            target_sq = get_lsb_index(pushes);
            cur_move = move | (target_sq << 6);

            if (not_promoting) {
                //                      Double Pawn FLag
                cur_move |= ((target_sq == source_sq - 16) << DOUBLE_SHIFT);
                move_list.push_back(cur_move);
            } else {
                //                               Promotion
                move_list.push_back(cur_move | (q << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (b << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (n << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (r << PROMOTION_SHIFT));
            }

            pushes ^= (1ULL << target_sq);
        }

        move |= CAPTURE_FLAG;

        while (attacks) {
            target_sq = get_lsb_index(attacks);
            cur_move = move | (target_sq << TARGET_SHIFT);

            if (not_promoting) {
                //                               Enpassant FLag
                int enpassant_flag = (board.enPassantSquare == target_sq) << ENPASSANT_SHIFT;
                move_list.push_back(cur_move | enpassant_flag);
            } else {
                //                               Promotion
                move_list.push_back(cur_move | (q << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (b << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (n << PROMOTION_SHIFT));
                move_list.push_back(cur_move | (r << PROMOTION_SHIFT));
            }

            attacks ^= 1ULL << target_sq;
        }

        p_bb ^= 1ULL << source_sq;
    }
}

void generate_knight_moves_white(const Board &board, std::vector<int> &move_list) {
    U64 kn_bb = board.bitboards[N];
    while (kn_bb) {
        int source_sq = get_lsb_index(kn_bb);

        U64 attacks = knightAttackMasks[source_sq] & ~board.coloredPieces[WHITE];

        while (attacks) {
            int target_sq = get_lsb_index(attacks);

            bool capture_flag = (board.coloredPieces[BLACK] >> target_sq) & 1ULL;
            int move = encode_move(source_sq, target_sq, N, 0, capture_flag, 0, 0, 0);
            move_list.push_back(move);

            attacks ^= 1ULL << target_sq;
        }
        kn_bb ^= 1ULL << source_sq;
    }
}

void generate_knight_moves_black(const Board &board, std::vector<int> &move_list) {
    U64 kn_bb = board.bitboards[n];
    while (kn_bb) {
        int source_sq = get_lsb_index(kn_bb);

        U64 attacks = knightAttackMasks[source_sq] & ~board.coloredPieces[BLACK];

        while (attacks) {
            int target_sq = get_lsb_index(attacks);

            bool capture_flag = (board.coloredPieces[WHITE] >> target_sq) & 1ULL;
            int move = encode_move(source_sq, target_sq, n, 0, capture_flag, 0, 0, 0);
            move_list.push_back(move);

            attacks ^= 1ULL << target_sq;
        }
        kn_bb ^= 1ULL << source_sq;
    }
}

void generate_bishop_moves_white(const Board &board, std::vector<int> &move_list) {
    U64 b_bb = board.bitboards[B];
    while (b_bb) {
        int source_sq = get_lsb_index(b_bb);

        U64 attacks = get_bishop_attacks(source_sq, board.allPieces);
        attacks &= ~board.coloredPieces[WHITE];

        while (attacks) {
            int target_sq = get_lsb_index(attacks);

            bool capture_flag = (board.coloredPieces[BLACK] >> target_sq) & 1ULL;
            int move = encode_move(source_sq, target_sq, B, 0, capture_flag, 0, 0, 0);
            move_list.push_back(move);

            attacks ^= 1ULL << target_sq;
        }
        b_bb ^= 1ULL << source_sq;
    }
}

void generate_bishop_moves_black(const Board &board, std::vector<int> &move_list) {
    U64 b_bb = board.bitboards[b];
    while (b_bb) {
        int source_sq = get_lsb_index(b_bb);

        U64 attacks = get_bishop_attacks(source_sq, board.allPieces);
        attacks &= ~board.coloredPieces[BLACK];

        while (attacks) {
            int target_sq = get_lsb_index(attacks);

            bool capture_flag = (board.coloredPieces[WHITE] >> target_sq) & 1ULL;
            int move = encode_move(source_sq, target_sq, b, 0, capture_flag, 0, 0, 0);
            move_list.push_back(move);

            attacks ^= 1ULL << target_sq;
        }
        b_bb ^= 1ULL << source_sq;
    }
}

void generate_rook_moves_white(const Board &board, std::vector<int> &move_list) {
    U64 r_bb = board.bitboards[R];
    while (r_bb) {
        int source_sq = get_lsb_index(r_bb);

        U64 attacks = get_rook_attacks(source_sq, board.allPieces);
        attacks &= ~board.coloredPieces[WHITE];

        while (attacks) {
            int target_sq = get_lsb_index(attacks);

            bool capture_flag = (board.coloredPieces[BLACK] >> target_sq) & 1ULL;
            int move = encode_move(source_sq, target_sq, R, 0, capture_flag, 0, 0, 0);
            move_list.push_back(move);

            attacks ^= 1ULL << target_sq;
        }
        r_bb ^= 1ULL << source_sq;
    }
}

void generate_rook_moves_black(const Board &board, std::vector<int> &move_list) {
    U64 r_bb = board.bitboards[r];
    while (r_bb) {
        int source_sq = get_lsb_index(r_bb);

        U64 attacks = get_rook_attacks(source_sq, board.allPieces);
        attacks &= ~board.coloredPieces[BLACK];

        while (attacks) {
            int target_sq = get_lsb_index(attacks);

            bool capture_flag = (board.coloredPieces[WHITE] >> target_sq) & 1ULL;
            int move = encode_move(source_sq, target_sq, r, 0, capture_flag, 0, 0, 0);
            move_list.push_back(move);

            attacks ^= 1ULL << target_sq;
        }
        r_bb ^= 1ULL << source_sq;
    }
}

void generate_queen_moves_white(const Board &board, std::vector<int> &move_list) {
    U64 q_bb = board.bitboards[Q];
    while (q_bb) {
        int source_sq = get_lsb_index(q_bb);

        U64 attacks = get_bishop_attacks(source_sq, board.allPieces);
        attacks |= get_rook_attacks(source_sq, board.allPieces);
        attacks &= ~board.coloredPieces[WHITE];

        while (attacks) {
            int target_sq = get_lsb_index(attacks);

            bool capture_flag = (board.coloredPieces[BLACK] >> target_sq) & 1ULL;
            int move = encode_move(source_sq, target_sq, Q, 0, capture_flag, 0, 0, 0);
            move_list.push_back(move);

            attacks ^= 1ULL << target_sq;
        }
        q_bb ^= 1ULL << source_sq;
    }
}

void generate_queen_moves_black(const Board &board, std::vector<int> &move_list) {
    U64 q_bb = board.bitboards[q];
    while (q_bb) {
        int source_sq = get_lsb_index(q_bb);

        U64 attacks = get_bishop_attacks(source_sq, board.allPieces);
        attacks |= get_rook_attacks(source_sq, board.allPieces);
        attacks &= ~board.coloredPieces[BLACK];

        while (attacks) {
            int target_sq = get_lsb_index(attacks);

            bool capture_flag = (board.coloredPieces[WHITE] >> target_sq) & 1ULL;
            int move = encode_move(source_sq, target_sq, q, 0, capture_flag, 0, 0, 0);
            move_list.push_back(move);

            attacks ^= 1ULL << target_sq;
        }
        q_bb ^= 1ULL << source_sq;
    }
}

void generate_king_moves_white(const Board &board, std::vector<int> &move_list) {
    U64 k_bb = board.bitboards[K];
    int source_sq = get_lsb_index_safe(k_bb);  // undefined behavior if no king on board

    U64 attacks = kingAttackMasks[source_sq];
    attacks &= ~board.coloredPieces[WHITE];

    while (attacks) {
        int target_sq = get_lsb_index(attacks);

        bool capture_flag = (board.coloredPieces[BLACK] >> target_sq) & 1ULL;
        int move = encode_move(source_sq, target_sq, K, 0, capture_flag, 0, 0, 0);
        move_list.push_back(move);

        attacks ^= 1ULL << target_sq;
    }


    // castling

    //  Castle_wk | Castle_wq | Castle_bk | Castle_bq;

    // queenside
    bool queenside_rights = board.castleFlags & Castle_wq;
    bool no_blockers_queenside = !(castle_wq_mask & board.allPieces);
    bool queenside = queenside_rights && no_blockers_queenside;

    // kingside
    bool kingside_rights = board.castleFlags & Castle_wk;
    bool no_blockers_kingside = !(castle_wk_mask & board.allPieces);
    bool kingside = kingside_rights && no_blockers_kingside;

    if (queenside) {
        move_list.push_back(encode_move(source_sq, 2, K, 0, 0, 0, 0, 1));
    }

    if (kingside) {
        move_list.push_back(encode_move(source_sq, 6, K, 0, 0, 0, 0, 1));
    }
}

void generate_king_moves_black(const Board &board, std::vector<int> &move_list) {
    U64 k_bb = board.bitboards[k];
    int source_sq = get_lsb_index_safe(k_bb);  // undefined behavior if no king on board

    U64 attacks = kingAttackMasks[source_sq];
    attacks &= ~board.coloredPieces[BLACK];

    while (attacks) {
        int target_sq = get_lsb_index(attacks);

        bool capture_flag = (board.coloredPieces[WHITE] >> target_sq) & 1ULL;
        int move = encode_move(source_sq, target_sq, k, 0, capture_flag, 0, 0, 0);
        move_list.push_back(move);

        attacks ^= 1ULL << target_sq;
    }

    // castling

    //  Castle_wk | Castle_wq | Castle_bk | Castle_bq;

    // queenside
    bool queenside_rights = board.castleFlags & Castle_bq;
    bool no_blockers_queenside = !(castle_bq_mask & board.allPieces);
    bool queenside = queenside_rights && no_blockers_queenside;

    // kingside
    bool kingside_rights = board.castleFlags & Castle_bk;
    bool no_blockers_kingside = !(castle_bk_mask & board.allPieces);
    bool kingside = kingside_rights && no_blockers_kingside;

    if (queenside) {
        move_list.push_back(encode_move(source_sq, 2 + 56, k, 0, 0, 0, 0, 1));
    }

    if (kingside) {
        move_list.push_back(encode_move(source_sq, 6 + 56, k, 0, 0, 0, 0, 1));
    }
}


inline bool is_king_exposed(const Board &bd, bool side) {
    return is_square_attacked(
        bd,
        get_lsb_index(bd.bitboards[piece_select(k, side)]),
        !side
    );
}


/*
void partition(std::vector<int> &move_list) {
    const int size = move_list.size();

    if (size < 2) {
        return;
    }

    int cap_i = 0;
    int noncap_i = 0;

    while (true) {
        while (move_list[cap_i] & CAPTURE_FLAG) {
            cap_i++;
            if (cap_i == size) {
                return;
            }
        }
        if (cap_i > noncap_i) {
            noncap_i = cap_i;
        }
        
        while (!(move_list[noncap_i] & CAPTURE_FLAG)) {
            noncap_i++;
            if (noncap_i == size) {
                return;
            }
        }
        std::swap(move_list[cap_i], move_list[noncap_i]);
    }
}
*/

void order_capture_first(std::vector<int> &move_list) {
    std::partition(
        move_list.begin(),
        move_list.end(),
        [](int move) { return move & CAPTURE_FLAG; }
    );
}

void generate_moves(const Board &board, std::vector<int> &move_list) {
    move_list.clear();

    // maybe one if statement for white or black instead of lots of branchless stuff?
    // profile both later

    if (board.turn) {
        // white
        generate_pawn_moves_white(board, move_list);
        generate_knight_moves_white(board, move_list);
        generate_bishop_moves_white(board, move_list);
        generate_rook_moves_white(board, move_list);
        generate_queen_moves_white(board, move_list);
        generate_king_moves_white(board, move_list);
    } else {
        // black
        generate_pawn_moves_black(board, move_list);
        generate_knight_moves_black(board, move_list);
        generate_bishop_moves_black(board, move_list);
        generate_rook_moves_black(board, move_list);
        generate_queen_moves_black(board, move_list);
        generate_king_moves_black(board, move_list);
    }

    // 2-4 times faster with this!!!
    order_capture_first(move_list);
}


bool make_move(Board &board, int move) {
    int source_sq = get_move_source(move);
    int target_sq = get_move_target(move);
    int piece = get_move_piece(move);
    int promotion = get_promotion(move);
    bool capture = get_capture_flag(move);
    bool double_shift = get_double_push_flag(move);
    bool enpassant = get_enpassant_flag(move);
    bool castle = get_castle_flag(move);

    bool side = board.turn;

    // move piece on appropriate bitboard
    board.bitboards[piece] ^= (1ULL << source_sq) | (1ULL << target_sq);

    board.castleFlags &= castling_rights[source_sq];

    board.halfMoveClock++;
    board.move += !side;

    if (capture) {
        // clear captured piece
        int start = piece_select(p, !side);

        U64 mask = ~(1ULL << target_sq);
        for (int i=start; i < start+6; i++) {
            board.bitboards[i] &= mask;
        }
        board.castleFlags &= castling_rights[target_sq];
        board.halfMoveClock = 0;
    }

    if (piece ==0 || piece == 6) {
        board.halfMoveClock = 0;
    }

    if (promotion) {
        board.bitboards[piece_select(p, side)] ^= 1ULL << target_sq;
        board.bitboards[promotion] ^= 1ULL << target_sq;
    }
    


    if (double_shift) {
        board.enPassantSquare = static_cast<Square>((source_sq + target_sq) / 2);
        //board.enPassantSquare = static_cast<Square>(source_sq + (side*16 - 8));
    } else {
        board.enPassantSquare = No_Square;
    }

    if (enpassant) {
        board.bitboards[piece_select(p, !side)] ^= 1ULL << (target_sq + 8 - side*16);
    }

    bool king_danger_illegal = 0;

    if (castle) {
        switch (target_sq) {
        case C1:
            board.bitboards[R] ^= 1ULL | (1ULL << D1);

            king_danger_illegal = is_square_attacked(board, D1, BLACK)
                                || is_square_attacked(board, E1, BLACK);
            break;
        case G1:
            board.bitboards[R] ^= (1ULL << H1) | (1ULL << F1);

            king_danger_illegal = is_square_attacked(board, F1, BLACK)
                                || is_square_attacked(board, E1, BLACK);
            break;
        case C8:
            board.bitboards[r] ^= (1ULL << A8) | (1ULL << D8);

            king_danger_illegal = is_square_attacked(board, D8, WHITE)
                                || is_square_attacked(board, E8, WHITE);
            break;
        case G8:
            board.bitboards[r] ^= (1ULL << H8) | (1ULL << F8);

            king_danger_illegal = is_square_attacked(board, F8, WHITE)
                                || is_square_attacked(board, E8, WHITE);
            break;
        
        default:
            assert(false);             // catch in debug
            __builtin_unreachable();   // optimize in release
        }
    }

    if (king_danger_illegal) {
        return false;
    }

    board.coloredPieces[BLACK] = board.bitboards[p]
                               | board.bitboards[r]
                               | board.bitboards[n]
                               | board.bitboards[b]
                               | board.bitboards[q]
                               | board.bitboards[k];

    board.coloredPieces[WHITE] = board.bitboards[P]
                               | board.bitboards[R]
                               | board.bitboards[N]
                               | board.bitboards[B]
                               | board.bitboards[Q]
                               | board.bitboards[K];
    
    
    board.allPieces = board.coloredPieces[BLACK] | board.coloredPieces[WHITE];
    board.turn ^= 1;

    return !is_king_exposed(board, side);
}


/*
Board make_capture_move(Board &board, int move) {
    if (get_capture_flag(move)) {
        return make_move(board, move);
    }

    
}
*/



std::array<int, 12> MATERIAL_SCORE = {
    -100,     // black pawn
    -500,     // black rook
    -320,     // black knight
    -350,     // black bishop
    -900,    // black queen
    -10000,   // black king
    100,      // white pawn
    500,      // white rook
    320,      // white knight
    350,      // white bishop
    900,     // white queen
    10000,    // white king
};


constexpr std::array<char, 64> PAWN_SQUARES = {
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
};
constexpr std::array<char, 64> ROOK_SQUARES = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
};
constexpr std::array<char, 64> KNIGHT_SQUARES = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};
constexpr std::array<char, 64> BISHOP_SQUARES = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
};
constexpr std::array<char, 64> QUEEN_SQUARES = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, -5,
    0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};
constexpr std::array<char, 64> KING_SQUARES = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 30, 10,  0,  0, 10, 30, 20
};

constexpr std::array<std::array<char, 64>, 6> PIECE_SQUARES = {
    PAWN_SQUARES,
    ROOK_SQUARES,
    KNIGHT_SQUARES,
    BISHOP_SQUARES,
    QUEEN_SQUARES,
    KING_SQUARES
};

inline int flip(int square) {
    return square ^ 56;
}


int evaluate(const Board &board) {
    int total = 0;
    bool side = board.turn;
    int sign = side * 2 - 1;

    
    for (int piece=0; piece<6; piece++) {
        U64 bb = board.bitboards[piece];

        while (bb) {
            int square = get_lsb_index(bb);
            bb ^= 1ULL << square;
            total += MATERIAL_SCORE[piece];

            total -= PIECE_SQUARES[piece][square];
        }
    }

    for (int piece=6; piece<12; piece++) {
        U64 bb = board.bitboards[piece];

        while (bb) {
            int square = get_lsb_index(bb);
            bb ^= 1ULL << square;
            total += MATERIAL_SCORE[piece];

            square = flip(square);
            total += PIECE_SQUARES[piece-6][square];
        }
    }

    // for negamax posative means good, negative means bad, for both players
    return total * sign;
}



void printAllAttackedSquares_stream(const Board &board, bool side, std::ostream& out) {
    for (int row = 7; row >= 0; --row) {
        out << row + 1 << "  ";
        for (int col = 0; col < 8; ++col) {
            int index = row * 8 + col;
            if (is_square_attacked(board, index, side)) {
                out << "X ";
            } else {
                out << ". ";
            }
        }
        out << std::endl;
    }
    out << "\n   A B C D E F G H" << std::endl;
}

void printAllAttackedSquares(const Board &board, bool side) {
    printAllAttackedSquares_stream(board, side, std::cout);
}



void print_attack_tables() {
    //U64 blockers = (1ULL << E2) | (1ULL << E6) | (1ULL << E5);
    U64 blockers = (1ULL << G2) | (1ULL << E6) | (1ULL << D5);

    printBitBoard(get_bishop_attacks(E4, blockers));
}

