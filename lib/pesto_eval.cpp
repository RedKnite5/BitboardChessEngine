#include "pesto_eval.h"
#include "board.h"
#include "evaluate_tables.h"

#include <array>

// https://chessprogramming.org/PeSTO%27s_Evaluation_Function

constexpr std::array<int, 12> gamephaseInc = {0, 1, 1, 2, 4, 0, 0, 1, 1, 2, 4, 0};

constexpr int get_lsb_index(unsigned long long x) {
    return __builtin_ctzll(x);
}

int pesto_eval(const Board board) {
    constexpr int whiteIndex = 0;
    constexpr int blackIndex = 1;

    std::array<int, 2> mg{};
    std::array<int, 2> eg{};
    int gamePhase = 0;

    for (Piece pc=p; pc<=K; ++pc) {
        uint64_t bb = board.bitboards[pc];
        const int color = pc < P ? blackIndex : whiteIndex;

        while (bb) {
            int sq = get_lsb_index(bb);
            bb ^= 1ULL << sq;
            mg[color] += mg_table[pc][sq];
            eg[color] += eg_table[pc][sq];
            gamePhase += gamephaseInc[pc];
        }
    }

    /* tapered eval */
    int mgScore = mg[!board.turn] - mg[board.turn];
    int egScore = eg[!board.turn] - eg[board.turn];
    int mgPhase = gamePhase;
    if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
    int egPhase = 24 - mgPhase;


    // printf("mg[WHITE]: %d  mg[BLACK]: %d\n", mg[WHITE], mg[BLACK]);
    // printf("mgScore: %d mgPhase: %d, egScore: %d egPhase: %d\n", mgScore, mgPhase, egScore, egPhase);

    return (mgScore * mgPhase + egScore * egPhase) / 24;
}

