
#include "search.h"
#include "engine_worker.h"


void EngineWorker::calculateMove(Game state) {
    // depth 9 > 2200 elo
    const int depth = 9;

    Game state_copy = state;

    long long start = current_time_us();

    int move = state.S.negamax(state.board, depth);
    //move = S.pv[0];
    long long duration = current_time_us() - start;

    print_move(move);

    printf("\n\nTime: %lld ms\n", duration / 1000);
    printf("Time: %lld s\n", duration / 1000000);
    printf("Nodes: %lld\n", state.S.nodes);

    // printf("Principal Variation:\n");
    // for (int i=0; i<MAX_PLY-1; i++) {
    //     print_move(state.S.pv[i]);
    // }


    // long long start_iterative_deepening = current_time_us();

    // int move_it_dep = 0;
    // for (int i=1; i<=depth; i++) {
    //     move_it_dep = state_copy.S.negamax(state_copy.board, i);
    // }
    // //move = S.pv[0];
    // long long duration_it_dep = current_time_us() - start_iterative_deepening;


    // printf("\nIterative deepening:\n");
    // print_move(move_it_dep);
    // printf("\n\nTime: %lld ms\n", duration_it_dep / 1000);
    // printf("Time: %lld s\n", duration_it_dep / 1000000);
    // printf("Nodes: %lld\n\n", state_copy.S.nodes);
    // printf("Principal Variation It Dep:\n");
    // for (int i=0; i<MAX_PLY-1; i++) {
    //     print_move(state_copy.S.pv[i]);
    // }

    emit moveReady(move);
}
