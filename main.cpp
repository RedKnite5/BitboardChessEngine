//#include "mainwindow.h"

//#include <QApplication>


#include <benchmark/benchmark.h>
#include <time.h>
#include <math.h>

#include "board.cpp"


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
    return nodes;
}

/*
Board bd(start_position);

static void BM_Driver(benchmark::State& state) {
    for (auto _ : state) {
        // Make sure driver() isn't optimized away
        long nodes = driver(bd, 6);
        benchmark::DoNotOptimize(nodes);
    }
}

BENCHMARK(BM_Driver)
    ->Iterations(5)                       // exactly 5 runs
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
*/

int main() {
    //init_all();

    //const char *position = "5B2/8/8/8/2P3R1/8/8/8 w - - 0 1";
    //const char *position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
    //const char *position = "2q5/3P4/8/6pP/8/r1b5/1P6/8 w - g6 0 1";
    //const char *position = "k7/7P/8/3pP3/8/K7/8/8 w - d6 0 1";
    //const char *position = "k7/7P/8/8/8/K7/p7/8 b - - 0 1";
    //const char *position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1 ";
    //Board bd = Board(position);
    //printBoard(bd);
    //std::cout << "Attacked: " << is_square_attacked(bd, E1, white) << std::endl;

    //Board bd = Board();

    printf("Start!\n");
    

    
    Board bd = Board(start_position);

    std::vector<long long> durations;

    int iterations = 1;

    long nodes = 0;

    for (int i=0; i<iterations; i++) {
        long long start = current_time_us();
        nodes = driver(bd, 7);
        durations.push_back(current_time_us() - start);
    }

    printf("nodes: %ld\n", nodes);

    long long tot_time = 0;

    printf("Times:\n");
    for (auto d : durations) {
        printf("%lld ms\n", d / 1000);
        tot_time += d;
    }

    long long mean = tot_time / iterations;

    printf("\nMean: %lld us\n", mean);
    printf("\nMean: %lld ms\n", mean / 1000);

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




    //printf("nodes: %ld\ntime: %lld\n", nodes, duration);

    


    //U64 blockers = (1ULL << G2) | (1ULL << E6) | (1ULL << D5) | (1ULL << E2) | (1ULL << E6) | (1ULL << E5);
    //U64 res = get_queen_attacks(E4, blockers);
    //printBitBoard(res);






    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();
}


