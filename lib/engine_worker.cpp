
#include "search.h"
#include "engine_worker.h"


void EngineWorker::calculateMove(Game state, int depth) {
    long long start = current_time_us();

    Move move = state.S.negamax(state.board, depth);
    //move = S.pv[0];
    long long duration = current_time_us() - start;

    move.print_move();

    printf("\n\nTime: %lld ms\n", duration / 1000);
    printf("Time: %lld s\n", duration / 1000000);
    printf("Nodes: %lld\n", state.S.nodes);

    // printf("Principal Variation:\n");
    // for (int i=0; i<MAX_PLY-1; i++) {
    //     print_move(state.S.pv[i]);
    // }

    // long long node_counts[128] = {0};
    // long long start_iterative_deepening = current_time_us();

    // int move_it_dep = 0;
    // for (int i=1; i<=depth; i++) {
    //     move_it_dep = state_copy.S.negamax(state_copy.board, i);
    //     node_counts[i] = state_copy.S.nodes;
    // }
    // //move = S.pv[0];
    // long long duration_it_dep = current_time_us() - start_iterative_deepening;


    // printf("\nIterative deepening:\n");
    // print_move(move_it_dep);
    // printf("Time: %lld ms\n", duration_it_dep / 1000);
    // printf("Time: %lld s\n", duration_it_dep / 1000000);
    // long long tot_nodes = 0;
    // for (int i=0; i<=depth; i++) {
    //     printf("Nodes at depth %d: %lld\n", i, node_counts[i]);
    //     tot_nodes += node_counts[i];
    // }
    // printf("Total Nodes: %lld\n\n", tot_nodes);

    // printf("Principal Variation It Dep:\n");
    // for (int i=0; i<MAX_PLY-1; i++) {
    //     print_move(state_copy.S.pv[i]);
    // }

    emit moveReady(move.m);
}
