#pragma once

#include <array>
#include <vector>

#include <QMetaType>

#include "board.h"

constexpr unsigned int MAX_PLY = 64;

constexpr int MIN_SCORE = -50000;
constexpr int MAX_SCORE = 50000;

constexpr unsigned int PV_SIZE = (MAX_PLY + 1) * MAX_PLY / 2;

long long current_time_us();

class Searcher {
    public:

    std::array<int, PV_SIZE> pv = {};
    long long nodes = 0;
    int ply = -1;

    std::array<std::array<int, MAX_PLY>, 2> killer_moves = {};
    std::array<std::array<short, 64>, 12> history_moves = {};

    int score_move(const Board &board, int move);
    void sort_moves(const Board &board, std::vector<int> &move_list);

    int quiscence(const Board &board, int alpha, int beta);
    int negamax_rec(const Board &board, int alpha, int beta, int depth);
    int negamax(const Board &board, int depth);
};


class Game {
    public:
    Searcher S;
    Board board;

    Game() = default;
    Game(const char *fen);
};
Q_DECLARE_METATYPE(Game)

