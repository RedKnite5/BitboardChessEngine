#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <atomic>

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
    uint8_t root_ply;  // age
    AlphaBeta flags;
};

constexpr int MB_TT = 1024*1024 / sizeof(TTEntry);

struct TT {
    static const int TableSize = 2*MB_TT;
    TTEntry tt[TableSize] = {};

    std::pair<int32_t, Move> probe(uint64_t key, int depth, int alpha, int beta) const;
    void set(uint64_t key, Move best, int depth, int value, int ply, AlphaBeta flags);

    TT() = default;

    // Delete the copy constructor
    TT(const TT& other) = delete;

    // Delete the copy assignment operator
    TT& operator=(const TT& other) = delete;

    void reset();
};

struct Settings {
    long long start_time = 0;
    long long stop_time = 0;
    std::atomic<bool> *stopped = nullptr;
    bool time_set = false;

    void checkup();
    void set_stopped_ptr(std::atomic<bool> *aStopped);
};


class Searcher {
  public:
    TT tt;

    std::unordered_map<uint64_t, uint8_t> real_repititions = {};
    std::unordered_set<uint64_t> search_repititions = {};

    std::vector<IrreversableState> state_stack = {};
    Settings settings;
    long long nodes = 0;

    // measure cutoffs for testing purposes
    //std::array<uint32_t, 15> cutoffs = {};

    std::array<Move, PV_SIZE> pv = {};
    int ply = -1;

    std::array<std::array<Move, MAX_PLY>, 2> killer_moves = {};
    std::array<std::array<short, 64>, 12> history_moves = {};


    bool follow_pv = false;
    bool null_moved = false;

    Searcher() = default;
    Searcher(long long time);

    int score_move(const Board &board, Move move, Move tt_move) const;
    void sort_moves(const Board &board, std::vector<Move> &move_list, Move tt_move) const;
    void move_reset();
    bool check_following_pv(const std::vector<Move> &move_list) const;
    bool check_3_fold_rep(const Board &board) const;

    int null_move_pruning(Board &board, int depth, int beta);

    int quiscence(Board &board, int alpha, int beta);
    int negamax_rec(Board &board, int alpha, int beta, int depth);
    Move negamax(Board &board, int depth);

    void reset();
};


class Game {
  public:
    Searcher S;
    Board board;

    Game() = default;
    Game(const char *fen);
    Game(const char *fen, long long time);

    void reset();
};
Q_DECLARE_METATYPE(std::shared_ptr<Game>)

