#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <array>


using U64 = uint64_t;





const bool black = false;
const bool white = true;



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
const U64 not_a_file = 18374403900871474942ULL;

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

constexpr inline int get_lsb_index_safe(U64 bitBoard) {
    int is_zero = bitBoard == 0;
    int safe_ctz = get_lsb_index(bitBoard | is_zero);
    return safe_ctz - is_zero;
}



constexpr U64 generatePawnAttackMask(U64 square, bool color) {
    U64 mask = 0;
    if (color) { // White pawns
        mask |= (square << 9) & not_a_file; // Capture right
        mask |= (square << 7) & not_h_file; // Capture left
    } else {     // Black pawns
        mask |= (square >> 9) & not_h_file; // Capture left (white's perspective)
        mask |= (square >> 7) & not_a_file; // Capture right
    }
    return mask;
}
constexpr auto generatePawnAttackMasks() {
    std::array<std::array<U64, 64>, 2> pawnAttackMasks = {};
    for (int i = 0; i < 64; ++i) {
        pawnAttackMasks[white][i] = generatePawnAttackMask(1ULL << i, white);
        pawnAttackMasks[black][i] = generatePawnAttackMask(1ULL << i, black);
    }
    return pawnAttackMasks;
}
constexpr const auto pawnAttackMasks = generatePawnAttackMasks();

constexpr auto generatePawnPushMasks() {
    std::array<std::array<U64, 56>, 2> pawnPushMasks = {};
    for (unsigned int i=0; i<56; ++i) {
        pawnPushMasks[white][i] = 1ULL << (i + 8);
        if (i >= 8) {
            pawnPushMasks[black][i] = 1ULL << (i - 8);
        }
        if (i >= 8 && i <= 15) {
            pawnPushMasks[white][i] |= 1ULL << (i + 16);
        }
        if (i >= 48 && i <= 55) {
            pawnPushMasks[black][i] |= 1ULL << (i - 16);
        }
    }
    return pawnPushMasks;
}
constexpr auto pawnPushMasks = generatePawnPushMasks();

constexpr U64 generateKnightAttackMask(U64 square) {
    U64 mask = 0;
    mask |= (square << 15) & not_h_file; // Up-Left
    mask |= (square << 17) & not_a_file; // Up-Right
    mask |= (square >> 15) & not_a_file; // Down-Right
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
    mask |= (square << 1) & not_a_file; // Right
    mask |= (square >> 1) & not_h_file; // Left
    mask |= (square << 8);               // Up
    mask |= (square >> 8);               // Down
    mask |= (square << 9) & not_a_file;  // Up-Right
    mask |= (square << 7) & not_h_file;  // Up-Left
    mask |= (square >> 9) & not_h_file;  // Down-Left
    mask |= (square >> 7) & not_a_file;  // Down-Right
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

