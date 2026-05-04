#include "mainwindow.h"

#include <QApplication>


//#include <benchmark/benchmark.h>
#include <time.h>
#include <math.h>

#include <algorithm>
#include <vector>

#include "board.h"

#include "search.h"


long long current_time_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

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
    //if (depth == 6) {
    //    printf("\nFinishing depth 6 on:\n");
    //    printBoard(board);
    //}
    return nodes;
}

void time_driver() {
    //const char *position = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ";
    Board bd = Board(start_position);

    std::vector<long long> durations;

    int iterations = 2;

    long nodes = 0;
    for (int i=0; i<iterations; i++) {
        long long start = current_time_us();
        nodes = driver(bd, 7);
        durations.push_back(current_time_us() - start);
    }
    printf("Nodes: %ld\n", nodes);

    long long tot_time = 0;

    for (size_t d=0; d<durations.size(); d++) {
        printf("Time: %lld ms\n", durations[d] / 1000);
        tot_time += durations[d];
    }

    
    long long mean = tot_time / iterations;

    printf("\nMean: %lld us\n", mean);
    printf("Mean: %lld ms\n", mean / 1000);

    long long ssd = 0;

    for (auto d : durations) {
        ssd += (d - mean) * (d - mean);
    }

    if (iterations > 1) {
        long long var = ssd / (iterations - 1);

        printf("Variance: %lld us\n", var);
        printf("Variance: %lld ms\n", var / 1000000);

        double stdev = sqrt((double) var);
        printf("Std Dev: %f us\n", stdev);
        printf("Std Dev: %f ms\n", stdev / 1000);
    }

}

void perft_correctness() {
    //const char *position = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ";

    // depth 6 node count for tricky_position should be 8031647685
    //                                     currently is 8031712933
    Board bd = Board(tricky_position);

    int depths = 5;

    std::vector<long long> durations;
    std::vector<long long> node_counts;
    for (int i=0; i<=depths; i++) {
        printf("Starting driver depth: %d\n", i);
        long long start = current_time_us();
        node_counts.push_back(driver(bd, i));
        durations.push_back(current_time_us() - start);

        printf("Nodes: %10lld    Time: %lld ms\n", node_counts[i], durations[i] / 1000);
    }

}


void sort_moves(const Board &board, std::vector<int> &move_list);


int Searcher::score_move(const Board &board, int move) {
    if (get_capture_flag(move)) {
        return mvv_lva(board, move);
    } else {
        if (killer_moves[0][ply] == move) {
            return 64 * MOVE_SORTING_SCALE;
        } else if (killer_moves[1][ply] == move) {
            return 32 * MOVE_SORTING_SCALE;
        } else {
            return history_moves[get_move_piece(move)][get_move_target(move)];
        }
    }

    return 0;
}

void Searcher::sort_moves(const Board &board, std::vector<int> &move_list) {
    std::ranges::sort(move_list, {}, [this, &board](int move){ return -score_move(board, move); });
}


int Searcher::quiscence(const Board &board, int alpha, int beta) {
    // debug info
    nodes++;

    int eval_score = evaluate(board);

    if (eval_score >= beta) {
        // fail high
        return beta;
    }

    if (eval_score > alpha) {
        alpha = eval_score;
    }

    std::vector<int> move_list;
    move_list.reserve(32);
    generate_capture_moves(board, move_list);
    sort_moves(board, move_list);

    for (int move : move_list) {
        Board new_board = board;
        bool king_safe = make_move(new_board, move);
        if (!king_safe) {
            continue;
        }

        int score = -quiscence(new_board, -beta, -alpha);

        if (score >= beta) {
            // fail high
            return beta;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

int Searcher::negamax_rec(const Board &board, int alpha, int beta, int depth) {
    if (depth == 0) {
        return quiscence(board, alpha, beta);
    }
    // debug info
    nodes++;
    ply++;

    std::vector<int> move_list;
    move_list.reserve(32);
    generate_moves(board, move_list);
    sort_moves(board, move_list);

    //print_move_list(move_list);

    bool legal_moves = false;
    unsigned int pv_index = ply * (2 * MAX_PLY + 1 - ply) / 2;

    for (int move : move_list) {
        Board new_board = board;
        bool king_safe = make_move(new_board, move);
        if (!king_safe) {
            continue;
        }

        legal_moves = true;

        int score = -negamax_rec(new_board, -beta, -alpha, depth-1);

        if (score >= beta) {
            // fail high

            if (!get_capture_flag(move)) {
                killer_moves[1][ply] = killer_moves[0][ply];
                killer_moves[0][ply] = move;
            }

            ply--;
            return beta;
        }

        if (score > alpha) {

            //if (!get_capture_flag(move)) {
                int piece = get_move_piece(move);
                int target = get_move_target(move);
                history_moves[piece][target] += depth;
            //}

            alpha = score;

            pv[pv_index] = move;
            std::copy_n(
                pv.begin() + pv_index + MAX_PLY - ply,
                MAX_PLY - ply - 1,
                pv.begin() + pv_index + 1
            );

        }
    }

    ply--;

    if (!legal_moves) {
        if (is_king_exposed(board, board.turn)) {
            // checkmake
            return MIN_SCORE + 1000 + board.move;
        } else {
            // stalemate
            return 0;
        }
    }

    return alpha;
}

int Searcher::negamax(const Board &board, int depth) {
    nodes = 0;

    int alpha = MIN_SCORE;
    int beta = MAX_SCORE;

    for (int current_depth = 1; current_depth <= depth; current_depth++) {
        negamax_rec(board, alpha, beta, current_depth);
    }

    return pv[0];
}

Game::Game(const char *fen) {
    if (fen) {
        board = Board(fen);
    } else {
        board = Board();
    }
    Searcher S;
}


void print_pv(const std::array<int, PV_SIZE> &pv) {
    printf("Principal Variation\n");
    for (unsigned int i=0; i<MAX_PLY; i++) {
        if (pv[i] == 0) {
            break;
        }
        print_move(pv[i]);
    }
}

void play(const char *fen, int moves, int depth) {
    Board board = Board(fen);
    Searcher S;
    printBoard(board);
    
    for (int i = 0; i < moves; i++) {
        //int look_ahead = 7;
        int mult = -1;
        if (board.turn) {
            //look_ahead = 6;
            mult = 1;
        }



        long long start = current_time_us();
        int move = S.negamax(board, depth);
        //move = S.pv[0];
        long long duration = current_time_us() - start;

        printf("\n\nTime: %lld ms\n", duration / 1000);
        printf("Time: %lld s\n", duration / 1000000);

        if (move) {
            printf("move:\n");
            print_move(move);
            make_move(board, move);
            printBoard(board);
            int score = -evaluate(board);
            printf("score: %d\n", score*mult);
            printf("Nodes: %lld\n", S.nodes);
            print_pv(S.pv);
        } else {
            printf("No moves!\n");

            if (is_king_exposed(board, board.turn)) {
                printf("Checkmate!\n");
            } else {
                printf("Stalemate!\n");
            }
            break;
        }
    }
}


//int main(int argc, char **argv) {
    //init_all();

    //const char *position = "5B2/8/8/8/2P3R1/8/8/8 w - - 0 1";
    //const char *position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
    //const char *position = "2q5/3P4/8/6pP/8/r1b5/1P6/8 w - g6 0 1";
    //const char *position = "k7/7P/8/3pP3/8/K7/8/8 w - d6 0 1";
    //const char *position = "k7/7P/8/8/8/K7/p7/8 b - - 0 1";
    //const char *position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1 ";
    //const char *position = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ";
    // const char *position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";

    // printf("Start!\n");

    // const char *fen = "1rb5/1p2k2r/p5n1/2p1pp2/2B5/6P1/PPPB1PP1/2KR4 w - - 1 0";

    //play(start_position, 15, 8);


    // const char *many_attacks = "4k3/1pppp3/2rRnbpr/p2PQ1B1/1PP2p1n/1Nq5/PBRPpPPP/1NbK4 b - - 0 1";

    // Board board = Board(many_attacks);
    // std::vector<int> move_list;
    // generate_moves(board, move_list);
    // std::ranges::sort(move_list, {}, [&board](int move){ return -score_move(board, move); });


    // for (auto move: move_list) {
    //     print_move(move);
    //     printf("Score: %d\n\n", score_move(board, move));
    // }
    

    // depth 6 node count for tricky_position should be 8031647685
    //                                     currently is 8031712933
    //Board bd = Board(position);

    
    //perft_correctness();
    //time_driver();

    // Searcher S;

    // int move = S.negamax(board, 3);
    // printf("move:\n");
    // print_move(move);

//     return play_graphical(argc, argv, fen, 15, 8);
    
// }


