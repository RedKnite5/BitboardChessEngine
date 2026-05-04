

//#include "AttackMasks.h"


//using U64 = uint64_t;




//const bool BLACK = false;
//const bool WHITE = true;



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
const U64 NOT_A_FILE = 18374403900871474942ULL;

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
const U64 not_h_file = 9187201950435737471ULL;

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
const U64 not_ab_files = 18229723555195321596ULL;

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
const U64 not_gh_files = 4557430888798830399ULL;

/*

// Bishop relivant occupancy bit counts
const int bishop_relevant_occupancy_bits[64] = {
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
const int rook_relevant_occupancy_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};


const U64 bishopMagicNumbers[64] = {
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

const U64 rookMagicNumbers[64] = {
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



constexpr inline int get_lsb_index_safe(U64 bitBoard) {
    int is_zero = bitBoard == 0;
    int safe_ctz = __builtin_ctzll(bitBoard | is_zero);
    return safe_ctz - is_zero;
}



constexpr U64 generatePawnAttackMask(U64 square, bool color) {
    U64 mask = 0;
    if (color) { // White pawns
        mask |= (square << 9) & NOT_A_FILE; // Capture right
        mask |= (square << 7) & not_h_file; // Capture left
    } else {     // Black pawns
        mask |= (square >> 9) & not_h_file; // Capture left (WHITE's perspective)
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
constexpr const auto pawnAttackMasks = generatePawnAttackMasks();

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
constexpr auto pawnPushMasks = generatePawnPushMasks();

constexpr U64 generateKnightAttackMask(U64 square) {
    U64 mask = 0;
    mask |= (square << 15) & not_h_file; // Up-Left
    mask |= (square << 17) & NOT_A_FILE; // Up-Right
    mask |= (square >> 15) & NOT_A_FILE; // Down-Right
    mask |= (square >> 17) & not_h_file; // Down-Left
    mask |= (square << 6) & not_gh_files;  // Left-Up
    mask |= (square << 10) & not_ab_files; // Right-Up
    mask |= (square >> 6) & not_ab_files;  // Right-Down
    mask |= (square >> 10) & not_gh_files; // Left-Down
    return mask;
}
constexpr std::array<U64, 64> generateKnightAttackMasks() {
    std::array<U64, 64> knightAttackMasks = {};
    for (int i = 0; i < 64; ++i) {
        knightAttackMasks[i] = generateKnightAttackMask(1ULL << i);
    }
    return knightAttackMasks;
}
constexpr std::array<U64, 64> knightAttackMasks = generateKnightAttackMasks();

constexpr U64 generateKingAttackMask(U64 square) {
    U64 mask = 0;
    mask |= (square << 1) & NOT_A_FILE; // Right
    mask |= (square >> 1) & not_h_file; // Left
    mask |= (square << 8);               // Up
    mask |= (square >> 8);               // Down
    mask |= (square << 9) & NOT_A_FILE;  // Up-Right
    mask |= (square << 7) & not_h_file;  // Up-Left
    mask |= (square >> 9) & not_h_file;  // Down-Left
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
constexpr std::array<U64, 64> kingAttackMasks = generateKingAttackMasks();


struct AttackTables {
    std::array<U64, 64> bishopPsudoAttackMasks;
    std::array<U64, 64> rookPsudoAttackMasks;
    std::array<std::array<U64, 512>, 64> bishopAttackMasks;
    std::array<std::array<U64, 4096>, 64> rookAttackMasks;
};

constexpr U64 generateBishopRelevantOccupancyMask(int square) {
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

constexpr U64 generateBishopAttackMask(int square, U64 occupied) {
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

constexpr U64 generateRookRelevantOccupancyMask(int square) {
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

constexpr U64 generateRookAttackMask(int square, U64 occupied) {
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

constexpr AttackTables attackTables = generateSliderAttackMasks();
constexpr auto &bishopPsudoAttackMasks = attackTables.bishopPsudoAttackMasks;  // [square]
constexpr auto &rookPsudoAttackMasks = attackTables.rookPsudoAttackMasks;      // [square]
constexpr auto &bishopAttackMasks = attackTables.bishopAttackMasks;   // [square][occupancy variation]
constexpr auto &rookAttackMasks = attackTables.rookAttackMasks;       // [square][occupancy variation]

*/
