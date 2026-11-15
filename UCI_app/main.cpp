#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <stdio.h>
#include <sstream>
#include <cstdlib>

#include "search.h"
#include "board.h"

using namespace std;

const char *name = "RED";



void run_engine(Game *game) {

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

Game parse_position(string_view s) {
    Game game = Game();

    const string pos_str = "position ";
    s.remove_prefix(pos_str.length());

    if (s.starts_with("fen")) {
        game = Game(s.substr(4).data());
    }

    size_t moves_start = s.find(" moves ", 4);
    if (moves_start == string::npos) {
        printf("UCI board no moves\n");
        printBoard(game.board);
        return game;
    }

    s.remove_prefix(moves_start + 7);

    // parse moves
    std::vector<Move> move_list;
    unsigned i = 0;
    while (i+3 < s.length()) {
        int source_file = s[i] - 'A';
        int source_rank = s[i+1] - '1';
        int dest_file = s[i+2] - 'A';
        int dest_rank = s[i+3] - '1';

        i += 4;

        int source_sq = source_rank * 8 + source_file;
        int dest_sq = dest_rank * 8 + dest_file;
        Piece promotion;
        if (i < s.length()) {
            switch (s[i]) {
                case 'q': promotion = q; break;
                case 'r': promotion = r; break;
                case 'b': promotion = b; break;
                case 'n': promotion = n; break;
                default: promotion = p; i--;
            }
            i++;
        }
        i++;

        generate_moves(game.board, move_list);
        Move userMove = findUserMoveUnambiguous(source_sq, dest_sq, promotion, move_list);
        bool legal = game.board.make_move(userMove);
        if (!legal) {
            printf("Error Illegal Move: ");
            userMove.print_move();
            exit(1);
        }
    }

    printf("UCI board with moves\n");
    printBoard(game.board);

    return game;
}

int parse_go(string s) {
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

    array<int, 9> parameters = {};

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

    if (infinite) {
        return -1;
    }

    if (movetime != -1) {
        return movetime;
    }

    int total_time = 0;
    if (wtime != -1) {
        total_time += wtime;
    }






}

void UCI_loop() {
    string input;

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    Game input_game;
    Game game_state;

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
            input_game = parse_position(input);
        }

        if (input.starts_with("ucinewgame")) {
            input_game = parse_position("position startpos");
        }

        if (input.starts_with("go")) {
            parse_go(input);
        }

        if (input == "quit") {
            break;
        }

        if (input == "uci") {
            printf("id name %s\n", name);
            printf("id author RedKnite\n");
            printf("uciok\n");
            continue;
        }




    }

}



int main(int argc, char **argv) {
    //generate_attack_table_h_file();


    UCI_loop();
}


