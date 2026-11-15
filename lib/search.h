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

enum class AlphaBeta: uint8_t {
    None,
    Exact,
    Alpha,
    Beta,
};

struct TTEntry {
    uint32_t key;   // half the key. The other half is implied by the TT bucket
    int32_t value;
    Move best;      // 32 bits
    uint8_t depth;
    AlphaBeta flags;
};

constexpr int MB_TT = 1024*1024 / sizeof(TTEntry);

struct TT {
    static const int TableSize = 1*MB_TT;
    TTEntry tt[TableSize] = {};

    int32_t probe(uint64_t key, int depth, int alpha, int beta) const;
    void set(uint64_t key, int depth, int value, AlphaBeta flags);
};

struct Settings {
    long long stop_time = 0;
    bool stopped = false;
    bool time_set = false;

    void checkup();
};

class Searcher {
  public:
    TT tt;

    std::array<Move, PV_SIZE> pv = {};
    Settings settings;
    long long nodes = 0;
    int ply = -1;

    std::array<std::array<Move, MAX_PLY>, 2> killer_moves = {};
    std::array<std::array<short, 64>, 12> history_moves = {};

    bool follow_pv = false;
    bool null_moved = false;

    Searcher() = default;
    Searcher(long long time);

    int score_move(const Board &board, Move move) const;
    void sort_moves(const Board &board, std::vector<Move> &move_list) const;

    int quiscence(const Board &board, int alpha, int beta);
    int negamax_rec(Board &board, int alpha, int beta, int depth);
    Move negamax(Board &board, int depth);
};


class Game {
  public:
    Searcher S;
    Board board;

    Game() = default;
    Game(const char *fen);
    Game(const char *fen, long long time);
};
Q_DECLARE_METATYPE(Game)

