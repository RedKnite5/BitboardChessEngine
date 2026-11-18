#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <functional>

#include "search.h"
#include "board.h"

using namespace std;

const char *name = "RED";



void run_engine(Game &game, atomic<bool> *stopped, Move &return_move) {
    game.S.settings.set_stopped_ptr(stopped);
    // printf("starting search on this board:\n");
    // printBoard(game.board);
    return_move = game.S.negamax(game.board, 10);
    game.S.move_reset();
    printf("bestmove %s\n", return_move.format_uci().data());
}




Move findUserMoveUnambiguous(int userSource, int userDest, int userProm, const std::vector<Move> &move_list) {
    for (auto move : move_list) {
        int source_sq = move.get_move_source();
        int target_sq = move.get_move_target();
        int promotion = move.get_promotion();
        if (promotion > k) {
            promotion -= P;
        }

        if (source_sq == userSource && userDest == target_sq && userProm == promotion) {
            return move;
        }
    }
    return Move(0);
}

Board parse_position(string_view s) {
    Board board = Board();

    const string pos_str = "position ";
    s.remove_prefix(pos_str.length());

    if (s.starts_with("fen")) {
        board = Board(s.substr(4).data());
    }

    size_t moves_start = s.find(" moves ", 4);
    if (moves_start == string::npos) {
        printf("UCI board no moves\n");
        printBoard(board);
        return board;
    }

    s.remove_prefix(moves_start + 7);

    // parse moves
    std::vector<Move> move_list;
    unsigned i = 0;
    while (i+3 < s.length()) {
        int source_file = s[i] - 'a';
        int source_rank = s[i+1] - '1';
        int dest_file = s[i+2] - 'a';
        int dest_rank = s[i+3] - '1';

        i += 4;

        int source_sq = source_rank * 8 + source_file;
        int dest_sq = dest_rank * 8 + dest_file;
        Piece promotion = p;
        if (i < s.length()) {
            switch (s[i]) {
                case 'q': promotion = q; break;
                case 'r': promotion = r; break;
                case 'b': promotion = b; break;
                case 'n': promotion = n; break;
                default: i--;
            }
            i++;
        }
        i++;

        printf("source: %s, dest: %s\n", square_coords[source_sq], square_coords[dest_sq]);

        generate_moves(board, move_list);
        Move userMove = findUserMoveUnambiguous(source_sq, dest_sq, promotion, move_list);
        bool legal = board.make_move(userMove);
        if (!legal) {
            printf("Error Illegal Move: ");
            userMove.print_move();
            exit(1);
        }
    }

    printf("UCI board with moves\n");
    printBoard(board);

    return board;
}

int parse_go(string s, bool color) {
    istringstream iss(s);

    bool infinite = false;
    int depth = -1;
    int movetime = -1;

    int wtime = -1;
    int btime = -1;
    int winc = -1;
    int binc = -1;

    int movestogo = -1;
    int nodes = -1;
    int mate = -1;

    const array<string_view, 9> map = {
        "depth",
        "movetime",
        "wtime",
        "btime",
        "winc",
        "binc",
        "movestogo",
        "nodes",
        "mate"
    };

    array<int, 9> parameters = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    string token;
    iss >> token;  // clear "go"
    while (iss >> token) {
        if (token == "infinite") {
            infinite = true;
            break;
        }

        for (unsigned i = 0; i<map.size(); i++) {
            if (token == map[i]) {
                string num;
                iss >> num;
                parameters[i] = atoi(num.data());
            }
        }
    }

    depth = parameters[0];
    movetime = parameters[1];
    wtime = parameters[2];
    btime = parameters[3];
    winc = parameters[4];
    binc = parameters[5];
    movestogo = parameters[6];
    nodes = parameters[7];
    mate = parameters[8];

    if (infinite) {
        return -1;
    }

    if (movetime != -1) {
        return movetime;
    }

    int total_time = 0;
    if (color == WHITE) {
        if (wtime != -1) {
            total_time += wtime;
        }

        if (movestogo != -1) {
            total_time /= movestogo;
        } else {
            total_time /= 30;
        }

        if (winc != -1) {
            total_time += winc;
        }
    } else {
        if (btime != -1) {
            total_time += btime;
        }

        if (movestogo != -1) {
            total_time /= movestogo;
        } else {
            total_time /= 30;
        }

        if (binc != -1) {
            total_time += binc;
        }
    }

    return total_time;
}

void UCI_loop() {
    string input;

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    Game game;
    atomic<bool> stopped = false;
    Move engine_move = Move(0);

    std::thread t1;

    long long start_time = 0;
    long long stop_time = 0;

    while (true) {
        getline(cin, input);

        if (input == "\n") {
            continue;
        }

        if (input == "isready") {
            printf("readyok\n");
            continue;
        }

        if (input.starts_with("position")) {
            if (t1.joinable()) {
                t1.join();
            }
            game.board = parse_position(input);
        }

        if (input.starts_with("ucinewgame")) {
            if (t1.joinable()) {
                t1.join();
            }
            game.reset();
            game.board = parse_position("position startpos");
        }

        if (input.starts_with("go")) {
            int time = parse_go(input, game.board.turn) * 1000 - 1000;
            printf("parsed duration %d\n", time);

            if (t1.joinable()) {
                t1.join();
            }

            stopped = false;

            if (time != -1) {
                game.S.settings.time_set = true;
                start_time = current_time_us();
                stop_time = start_time + time;
                game.S.settings.stop_time = stop_time;
            }

            t1 = thread(run_engine, ref(game), &stopped, ref(engine_move));
        }

        if (input == "stop" || (stop_time != 0 && stop_time <= current_time_us())) {
            stopped = true;
            if (t1.joinable()) {
                t1.join();
            }
            
            start_time = 0;
            stop_time = 0;
        }

        if (input == "quit") {
            stopped = true;
            break;
        }

        if (input == "uci") {
            printf("id name %s\n", name);
            printf("id author RedKnite\n");
            printf("uciok\n");
            continue;
        }

        //sleep(1);
    }

    if (t1.joinable()) {
        t1.join();
    }
}



int main(int argc, char **argv) {
    //generate_attack_table_h_file();

    UCI_loop();
}


/*
c-chess-cli_linux -log -each movetime=3 option."Move Overhead"=2000 \
    -engine cmd=./UCI_app \
    -engine option.UCI_LimitStrength=true option.UCI_Elo=1500 option.Threads=1 cmd=~/chess_utils/stockfish/stockfish-linux-x86-64-universal

c-chess-cli_linux -log -each tc=20+.5 option."Move Overhead"=500 \
    -engine cmd=./UCI_app \
    -engine option.UCI_LimitStrength=true option.UCI_Elo=1500 option.Threads=1 cmd=~/chess_utils/stockfish/stockfish-linux-x86-64-universal

c-chess-cli_linux -sprt -games 10 -log -each tc=20+.5 option."Move Overhead"=500 \
    -engine option.UCI_LimitStrength=true option.UCI_Elo=1500 option.Threads=1 cmd=~/chess_utils/stockfish/stockfish-linux-x86-64-universal \
    -engine cmd=./UCI_app


Score of RED vs Stockfish 19: 4 - 3 - 3  [0.550] 10
SPRT: LLR = 0.016 [-2.944,2.944]


c-chess-cli_linux -sprt -games 2 -log -each tc=20+.5 option."Move Overhead"=500 \
    -engine option.UCI_LimitStrength=true option.UCI_Elo=1500 option.Threads=1 cmd=~/chess_utils/stockfish/stockfish-linux-x86-64-universal \
    -engine cmd=./UCI_app


c-chess-cli_linux -sprt -games 20 -each tc=20+.5 option."Move Overhead"=500 \
    -engine name=base cmd=./Engine_base \
    -engine name=age cmd=./Engine_with_age


Score of base vs age: 3 - 4 - 13  [0.475] 20
SPRT: LLR = -0.037 [-2.944,2.944]

*/
