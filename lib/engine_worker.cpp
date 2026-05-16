
#include "search.h"
#include "engine_worker.h"


void EngineWorker::calculateMove(Game state) {
    // depth 9 > 1500 elo
    const int depth = 9;

    long long start = current_time_us();
    int move = state.S.negamax(state.board, depth);
    //move = S.pv[0];
    long long duration = current_time_us() - start;

    print_move(move);

    printf("\n\nTime: %lld ms\n", duration / 1000);
    printf("Time: %lld s\n", duration / 1000000);

    emit moveReady(move);
}
