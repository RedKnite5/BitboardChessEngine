
#include "search.h"
#include "engine_worker.h"


void EngineWorker::calculateMove(Game state) {
    const int depth = 8;

    long long start = current_time_us();
    int move = game.S.negamax(game.board, depth);
    //move = S.pv[0];
    long long duration = current_time_us() - start;

    print_move(move);

    printf("\n\nTime: %lld ms\n", duration / 1000);
    printf("Time: %lld s\n", duration / 1000000);

    emit moveReady(move);
}
