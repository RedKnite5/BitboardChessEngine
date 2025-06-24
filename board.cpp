#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <cstring> // For memset
#include <random>  // For random number generation
#include <type_traits>

using U64 = uint64_t;


// FEN dedug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - 0 1"
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "


const bool black = false;
const bool white = true;
enum Square {
    A1 = 0, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, No_Square
};


/*
   not A file

8  . 1 1 1 1 1 1 1
7  . 1 1 1 1 1 1 1
6  . 1 1 1 1 1 1 1
5  . 1 1 1 1 1 1 1
4  . 1 1 1 1 1 1 1
3  . 1 1 1 1 1 1 1
2  . 1 1 1 1 1 1 1
1  . 1 1 1 1 1 1 1

   A B C D E F G H
*/
const U64 not_a_file = 18374403900871474942ULL;

/*
   not H file
8  1 1 1 1 1 1 1 .
7  1 1 1 1 1 1 1 .
6  1 1 1 1 1 1 1 .
5  1 1 1 1 1 1 1 .
4  1 1 1 1 1 1 1 .
3  1 1 1 1 1 1 1 .
2  1 1 1 1 1 1 1 .
1  1 1 1 1 1 1 1 .

   A B C D E F G H
*/
const U64 not_h_file = 9187201950435737471ULL;

/*
   not AB files
8  . . 1 1 1 1 1 1
7  . . 1 1 1 1 1 1
6  . . 1 1 1 1 1 1
5  . . 1 1 1 1 1 1
4  . . 1 1 1 1 1 1
3  . . 1 1 1 1 1 1
2  . . 1 1 1 1 1 1
1  . . 1 1 1 1 1 1

   A B C D E F G H
*/
const U64 not_ab_files = 18229723555195321596ULL;

/*
   not GH files
8  1 1 1 1 1 1 . .
7  1 1 1 1 1 1 . .
6  1 1 1 1 1 1 . .
5  1 1 1 1 1 1 . .
4  1 1 1 1 1 1 . .
3  1 1 1 1 1 1 . .
2  1 1 1 1 1 1 . .
1  1 1 1 1 1 1 . .

   A B C D E F G H
*/
const U64 not_gh_files = 4557430888798830399ULL;


// Bishop relivant occupancy bit counts
const int bishop_relevant_occupancy_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};
// Rook relivant occupancy bit counts
const int rook_relevant_occupancy_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};


const U64 bishopMagicNumbers[64] = {
    5811437957751537696,
    2269413488787630,
    4504226696791041,
    655287494014666753,
    2378199945829286161,
    36363117541720840,
    36320236355453952,
    4630263508761084931,
    1152926057339814416,
    5877219538881233408,
    5350566636981454888,
    180216595845480704,
    27026065218076768,
    20268543914278912,
    286440009777218,
    6926536502881558528,
    18032613499470882,
    4504200992526528,
    1443017855103683072,
    90230356615249920,
    1155315158644424964,
    1297318176257868033,
    6197093833508930053,
    18058658156381248,
    23098541351372800,
    586066086023661696,
    36064532288241792,
    29836897421657152,
    9012696846467584,
    283674016778372,
    12385998564528132,
    306456430683128832,
    9306884962590724,
    108235941885775968,
    567777500989440,
    2648125411346745856,
    18019896336646209,
    41100023894769800,
    1697654794944656,
    2306124819202114320,
    162412298781663776,
    585753880487272449,
    145152731531776,
    4521278048372736,
    36031408510075394,
    198175976872480964,
    29362462348280840,
    2260600204132544,
    1450444961932645376,
    2882585515935678529,
    2458965963493081088,
    5839479909873091648,
    2305931107620882433,
    1153097563939831829,
    2900335760813130272,
    2270732331532320,
    153404008604894228,
    2630243521445122049,
    288372213705869344,
    27625242362404,
    290851612428091649,
    576461027248984192,
    721289559998980,
    2252916513835264
};

const U64 rookMagicNumbers[64] = {
    36029213903423616,
    5782622058996301888,
    180179169602175104,
    72075186643443716,
    216177802932125698,
    5044049183435662341,
    36029347345203456,
    1224979261857759488,
    140776688599042,
    162411200074811520,
    4612249114418135568,
    140806249826304,
    4900057286686802048,
    562967905046664,
    73464977520066816,
    36310273146028034,
    76702480911646722,
    1211468574644789248,
    9078668047482896,
    144260873500758016,
    216195871992250384,
    216595544368219136,
    145245486163562501,
    630506146872053900,
    2306546700952600640,
    1166441375533637632,
    246294900654336,
    2251868801663488,
    360851066172215305,
    2314287267106065416,
    5188166579120310288,
    721139448678998276,
    18019346991284288,
    869265234336944129,
    580968750558218240,
    2307114079023534080,
    4755942510935672832,
    118220040049789440,
    1455225633922615372,
    45071731576013829,
    1734766290582929408,
    598203082817536,
    4620695966729666592,
    4504699275640848,
    1155226115355246596,
    576533320137998464,
    342284086330785800,
    37296058325401601,
    36029071940911168,
    37858659247587968,
    4683814118786138368,
    3458905285669683328,
    1224987894822797568,
    153685629479174656,
    158505905832960,
    2307043680215122432,
    2307039827622637825,
    72620819411403010,
    56086401666588673,
    36042029816348705,
    4612249312515654658,
    6953839325473671233,
    72339111966671489,
    36029089079429250
};


// Attack masks for pawns, indexed by color and square (0-47 for squares A2-H7)
// indeces 0-7 are empty as pawns cannot be on the first rank
U64 pawnAttackMasks[2][56];
U64 knightAttackMasks[64];
U64 kingAttackMasks[64];

U64 bishopPsudoAttackMasks[64];
U64 rookPsudoAttackMasks[64];

U64 bishopAttackMasks[64][512]; // [square][occupancy variation]
U64 rookAttackMasks[64][4096];  // [square][occupancy variation]

#define count_bits __builtin_popcountll

inline int get_lsb_index(U64 bitBoard) {
    if (bitBoard == 0) return -1; // No bits set
    return __builtin_ctzll(bitBoard); // Count trailing zeros
}

static U64 magic_number_candidate() {
    U64 n1 = random() | (random() << 32);
    U64 n2 = random() | (random() << 32);
    U64 n3 = random() | (random() << 32);
    return n1 & n2 & n3;
}

enum Pieces {
    r, n, b, q, k, p, // Black pieces
    R, N, B, Q, K, P,  // White pieces
    Last
};
const char ascii_pieces[] = "rnbqkpRNBQKP";
const char *unicode_pieces[12] = {"♖", "♘", "♗", "♕", "♔", "♙",
                            "♜", "♞", "♝", "♛", "♚", "♟"};


const unsigned char Castle_none = 0;
const unsigned char Castle_wk = 1 << 0;
const unsigned char Castle_wq = 1 << 1;
const unsigned char Castle_bk = 1 << 2;
const unsigned char Castle_bq = 1 << 3;


class Board {
  public:
    U64 bitboards[12];
    U64 whitePieces;
    U64 blackPieces;
    U64 allPieces;
    Square enPassantSquare = No_Square;
    unsigned short move;
    bool turn; // true for white, false for black
    unsigned char castleFlags;
    unsigned short halfMoveClock;
    Board() {
        move = 0;
        turn = white;
        castleFlags = Castle_wk | Castle_wq | Castle_bk | Castle_bq;
        halfMoveClock = 0;
        U64 whitePieces = 0xFFFF;
        U64 whitePawns = 0xFF << 8;
        U64 whiteRooks = 1 | (1 << 7);
        U64 whiteKnights = (1 << 1) | (1 << 6);
        U64 whiteBishops = (1 << 2) | (1 << 5);
        U64 whiteQueens = 1 << 3;
        U64 whiteKing = 1 << 4;
        U64 blackPieces = 0xFFFFULL << 48;
        U64 blackPawns = 0xFFULL << 48;
        U64 blackRooks = (1ULL << 56) | (1ULL << 63);
        U64 blackKnights = (1ULL << 57) | (1ULL << 62);
        U64 blackBishops = (1ULL << 58) | (1ULL << 61);
        U64 blackQueens = 1ULL << 59;
        U64 blackKing = 1ULL << 60;
        bitboards[0] =  blackRooks;
        bitboards[1] =  blackKnights;
        bitboards[2] =  blackBishops;
        bitboards[3] =  blackQueens;
        bitboards[4] =  blackKing;
        bitboards[5] =  blackPawns;
        bitboards[6] =  whiteRooks;
        bitboards[7] =  whiteKnights;
        bitboards[8] =  whiteBishops;
        bitboards[9] =  whiteQueens;
        bitboards[10] = whiteKing;
        bitboards[11] = whitePawns;

        whitePieces = whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing | whitePawns;
        blackPieces = blackRooks | blackKnights | blackBishops | blackQueens | blackKing | blackPawns;
        allPieces = whitePieces | blackPieces;
    }

    // Constructor to initialize from a FEN string
    Board(const char *str) {
        memset(bitboards, 0, sizeof(bitboards));
        int i = 0;
        int square = 56;
        bool piecePlacement = true;
        while (piecePlacement) {
            char c = str[i];
            switch (c) {
                case 'r':
                    bitboards[r] |= 1ULL << square;
                    blackPieces |= 1ULL << square;
                    break;
                case 'n':
                    bitboards[n] |= 1ULL << square;
                    blackPieces |= 1ULL << square;
                    break;
                case 'b':
                    bitboards[b] |= 1ULL << square;
                    blackPieces |= 1ULL << square;
                    break;
                case 'q':
                    bitboards[q] |= 1ULL << square;
                    blackPieces |= 1ULL << square;
                    break;
                case 'k':
                    bitboards[k] |= 1ULL << square;
                    blackPieces |= 1ULL << square;
                    break;
                case 'p':
                    bitboards[p] |= 1ULL << square;
                    blackPieces |= 1ULL << square;
                    break;
                case 'R':
                    bitboards[R] |= 1ULL << square;
                    whitePieces |= 1ULL << square;
                    break;
                case 'N':
                    bitboards[N] |= 1ULL << square;
                    whitePieces |= 1ULL << square;
                    break;
                case 'B':
                    bitboards[B] |= 1ULL << square;
                    whitePieces |= 1ULL << square;
                    break;
                case 'Q':
                    bitboards[Q] |= 1ULL << square;
                    whitePieces |= 1ULL << square;
                    break;
                case 'K':
                    bitboards[K] |= 1ULL << square;
                    whitePieces |= 1ULL << square;
                    break;
                case 'P':
                    bitboards[P] |= 1ULL << square;
                    whitePieces |= 1ULL << square;
                    break;
                case '/':
                    square -= 17;
                    break;
                case ' ':
                    //break out of the loop when we reach the end of the piece placement
                    piecePlacement = false;
                    break;
                default:
                    if (c >= '1' && c <= '8') {
                        square += (c - '0') - 1; // Move square back by the number of empty squares
                    } else {
                        std::cerr << "Invalid character in FEN string piece placement: " << c << std::endl;
                        exit(EXIT_FAILURE);
                    }
            }
            i++;
            square++;
        }

        if (str[i] == 'w') {
            move = 0; // White to move
        } else if (str[i] == 'b') {
            move = 1; // Black to move
        } else {
            std::cerr << "Invalid turn in FEN string turn: " << str[i] << std::endl;
            exit(EXIT_FAILURE);
        }

        i++;
        if (str[i] != ' ') {
            std::cerr << "Invalid character after turn in FEN string: " << str[i] << std::endl;
            exit(EXIT_FAILURE);
        }
        i++;

        castleFlags = Castle_none;
        bool castling = true;
        while (castling) {
            char c = str[i];
            switch (c) {
                case 'K':
                    castleFlags |= Castle_wk;
                    break;
                case 'Q':
                    castleFlags |= Castle_wq;
                    break;
                case 'k':
                    castleFlags |= Castle_bk;
                    break;
                case 'q':
                    castleFlags |= Castle_bq;
                    break;
                case '-':
                    i++;
                    castling = false;
                    break;
                case ' ':
                    castling = false;
                    break;
                default:
                    std::cerr << "Invalid character in castling rights: " << c << std::endl;
                    exit(EXIT_FAILURE);
            }
            i++;
        }

        if (str[i] == '-') {
            enPassantSquare = No_Square; // No en passant square
            i++;
        } else if (str[i] >= 'a' && str[i] <= 'h' && str[i + 1] >= '1' && str[i + 1] <= '8') {
            enPassantSquare = static_cast<Square>((str[i + 1] - '1') * 8 + (str[i] - 'a'));
            if (str[i+1] == '\0') {
                std::cerr << "Invalid en passant square in FEN string: " << str[i] << str[i + 1] << std::endl;
                exit(EXIT_FAILURE);
            }
            i += 2; // Move past the en passant square
        } else {
            std::cerr << "Invalid en passant square in FEN string at:" << i << " str[i] ='" << str[i] << "'str[i+1] ='" << str[i + 1] << '\'' << std::endl;
            exit(EXIT_FAILURE);
        }

        if (str[i] != ' ') {
            std::cerr << "Invalid character after en passant square in FEN string: " << str[i] << std::endl;
            exit(EXIT_FAILURE);
        }
        i++;

        // Half-move clock
        halfMoveClock = 0;
        while (str[i] >= '0' && str[i] <= '9') {
            halfMoveClock = halfMoveClock * 10 + (str[i] - '0');
            i++;
        }
        
        if (str[i] != ' ') {
            std::cerr << "Invalid character after half-move clock in FEN string: " << str[i] << std::endl;
            printf("str[i] = '%d'\n", str[i]);
            exit(EXIT_FAILURE);
        }
        i++;

        // Full-move number
        move = 0;
        while (str[i] >= '0' && str[i] <= '9') {
            move = move * 10 + (str[i] - '0');
            i++;
        }
        // Update allPieces
        allPieces = whitePieces | blackPieces;
    }
};

void printBitBoard(U64 bitBoard) {
    for (int row = 7; row >= 0; --row) {
        std::cout << row + 1 << "  ";
        for (int col = 0; col < 8; ++col) {
            int index = row * 8 + col;
            if (bitBoard & (1ULL << index)) {
                std::cout << "1 ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\n   A B C D E F G H" << std::endl;
    std::cout << bitBoard << std::endl;
}


void printBoard(Board board) {
    for (int row = 7; row >= 0; --row) {
        std::cout << row + 1 << "  ";
        for (int col = 0; col < 8; ++col) {
            int index = row * 8 + col;
            for (int piece = r; piece != Last; ++piece) {
                if (board.bitboards[piece] & (1ULL << index)) {
                    std::cout << unicode_pieces[piece] << " ";
                    break;
                }
                if (piece == P) { // If no piece found, print empty square
                    std::cout << ". ";
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\n   A B C D E F G H\n";
    std::cout << "Move: " << board.move << std::endl;
    std::cout << "Castle Flags: ";
    const char castling_key[] = "KQkq";
    for (int i = 0; i < 4; ++i) {
        if ((board.castleFlags & 1 << i) != Castle_none) {
            std::cout << castling_key[i];
        } else {
            std::cout << "-";
        }
    }
    std::cout << std::endl;
    std::cout << "En Passant Square: " << board.enPassantSquare << std::endl;
    std::cout << "Half Move Clock: " << board.halfMoveClock << std::endl;
}


U64 generatePawnAttackMask(U64 square, bool color) {
    U64 mask = 0;
    if (color) { // White pawns
        mask |= (square << 9) & not_a_file; // Capture right
        mask |= (square << 7) & not_h_file; // Capture left
    } else {     // Black pawns
        mask |= (square >> 9) & not_h_file; // Capture left (white's perspective)
        mask |= (square >> 7) & not_a_file; // Capture right
    }
    //printBoardState(mask);
    return mask;
}

void generatePawnAttackMasks() {
    for (int i = 8; i < 56; ++i) {
        pawnAttackMasks[white][i] = generatePawnAttackMask(1ULL << i, white);
        pawnAttackMasks[black][i] = generatePawnAttackMask(1ULL << i, black);
    }
}

U64 generateKnightAttackMask(U64 square) {
    U64 mask = 0;
    mask |= (square << 15) & not_h_file; // Up-Left
    mask |= (square << 17) & not_a_file; // Up-Right
    mask |= (square >> 15) & not_a_file; // Down-Right
    mask |= (square >> 17) & not_h_file; // Down-Left
    mask |= (square << 6) & not_gh_files;  // Left-Up
    mask |= (square << 10) & not_ab_files; // Right-Up
    mask |= (square >> 6) & not_ab_files;  // Right-Down
    mask |= (square >> 10) & not_gh_files; // Left-Down
    return mask;
}

void generateKnightAttackMasks() {
    for (int i = 0; i < 64; ++i) {
        knightAttackMasks[i] = generateKnightAttackMask(1ULL << i);
    }
}

U64 generateKingAttackMask(U64 square) {
    U64 mask = 0;
    mask |= (square << 1) & not_a_file; // Right
    mask |= (square >> 1) & not_h_file; // Left
    mask |= (square << 8);               // Up
    mask |= (square >> 8);               // Down
    mask |= (square << 9) & not_a_file;  // Up-Right
    mask |= (square << 7) & not_h_file;  // Up-Left
    mask |= (square >> 9) & not_h_file;  // Down-Left
    mask |= (square >> 7) & not_a_file;  // Down-Right
    return mask;
}

void generateKingAttackMasks() {
    for (int i = 0; i < 64; ++i) {
        kingAttackMasks[i] = generateKingAttackMask(1ULL << i);
    }
}

U64 generateBishopRelevantOccupancyMask(int square) {
    U64 mask = 0;

    int tr = square / 8;
    int tf = square % 8;

    for (int i = 1; i < 8; ++i) {
        if (tr + i < 7 && tf + i < 7) {
            // Up-Right
            mask |= (1ULL << ((tr + i) * 8 + (tf + i)));
        }
        if (tr - i >= 1 && tf - i >= 1) {
            // Down-Left
            mask |= (1ULL << ((tr - i) * 8 + (tf - i)));
        }
        if (tr + i < 7 && tf - i >= 1) {
            // Up-Left
            mask |= (1ULL << ((tr + i) * 8 + (tf - i)));
        }
        if (tr - i >= 1 && tf + i < 7) {
            // Down-Right
            mask |= (1ULL << ((tr - i) * 8 + (tf + i)));
        }
    }
    return mask;
}

U64 generateBishopAttackMask(int square, U64 occupied) {
    U64 mask = 0;

    int tr = square / 8;
    int tf = square % 8;

    for (int i = tr, j = tf; i < 8 && j < 8; ++i, ++j) {
        // Up-Right
        mask |= 1ULL << (i * 8 + j);
        if (occupied & (1ULL << (i * 8 + j))) {
            break; // Stop if occupied
        }
    }
    for (int i = tr, j = tf; i >= 0 && j >= 0; --i, --j) {
        // Down-Left
        mask |= 1ULL << (i * 8 + j);
        if (occupied & (1ULL << (i * 8 + j))) {
            break;
        }
    }
    for (int i = tr, j = tf; i < 8 && j >= 0; ++i, --j) {
        // Up-Left
        mask |= 1ULL << (i * 8 + j);
        if (occupied & (1ULL << (i * 8 + j))) {
            break;
        }
    }
    for (int i = tr, j = tf; i >= 0 && j < 8; --i, ++j) {
        // Down-Right
        mask |= 1ULL << (i * 8 + j);
        if (occupied & (1ULL << (i * 8 + j))) {
            break;
        }
    }
    // Remove the square itself from the mask
    mask ^= 1ULL << square;

    return mask;
}

U64 generateRookRelevantOccupancyMask(int square) {
    U64 mask = 0;

    int tr = square / 8;
    int tf = square % 8;

    for (int i = 1; i < 7; ++i) {
        if (i != tr) {
            // Vertical
            mask |= (1ULL << (i * 8 + tf));
        }
        if (i != tf) {
            // Horizontal
            mask |= (1ULL << (tr * 8 + i));
        }
    }
    return mask;
}

U64 generateRookAttackMask(int square, U64 occupied) {
    U64 mask = 0;

    int tr = square / 8;
    int tf = square % 8;

    for (int i = tr+1; i < 8; ++i) {
        // Vertical Up
        mask |= (1ULL << (i * 8 + tf));
        if (occupied & (1ULL << (i * 8 + tf))) {
            break; // Stop if occupied
        }
    }
    for (int i = tr-1; i >= 0; --i) {
        // Vertical Down
        mask |= (1ULL << (i * 8 + tf));
        if (occupied & (1ULL << (i * 8 + tf))) {
            break;
        }
    }
    for (int i = tf+1; i < 8; ++i) {
        // Horizontal Right
        mask |= (1ULL << (tr * 8 + i));
        if (occupied & (1ULL << (tr * 8 + i))) {
            break;
        }
    }
    for (int i = tf-1; i >= 0; --i) {
        // Horizontal Left
        mask |= (1ULL << (tr * 8 + i));
        if (occupied & (1ULL << (tr * 8 + i))) {
            break;
        }
    }

    return mask;
}

U64 setOccupancy(int index, int bits_in_mask, U64 mask) {
    // index goes up to 2^bits_in_mask
    U64 occupancy = 0;
    for (int i = 0; i < bits_in_mask; ++i) {
        int square = get_lsb_index(mask);
        mask &= mask - 1;
        if (index & (1 << i)) {
            occupancy |= (1ULL << square);
        }
    }
    return occupancy;
}

void generateSliderAttackMasks() {
    for (int square=0; square < 64; ++square) {

        bishopPsudoAttackMasks[square] = generateBishopRelevantOccupancyMask(square);
        rookPsudoAttackMasks[square] = generateRookRelevantOccupancyMask(square);

        int bishop_rel_occ_count = bishop_relevant_occupancy_bits[square];
        int bishop_occupancy_indeces = 1ULL << bishop_rel_occ_count;

        for (int i=0; i < bishop_occupancy_indeces; ++i) {
            U64 occupancy = setOccupancy(i, bishop_rel_occ_count, bishopPsudoAttackMasks[square]);
            int magic_index = (occupancy * bishopMagicNumbers[square]) >> (64 - bishop_rel_occ_count);
            bishopAttackMasks[square][magic_index] = generateBishopAttackMask(square, occupancy);
        }

        int rook_rel_occ_count = rook_relevant_occupancy_bits[square];
        int rook_occupancy_indeces = 1ULL << rook_relevant_occupancy_bits[square];

        for (int i=0; i < rook_occupancy_indeces; ++i) {
            U64 occupancy = setOccupancy(i, rook_rel_occ_count, rookPsudoAttackMasks[square]);
            int magic_index = (occupancy * rookMagicNumbers[square]) >> (64 - rook_rel_occ_count);
            rookAttackMasks[square][magic_index] = generateRookAttackMask(square, occupancy);
        }

    }
}

U64 magic_number(int square, int relivant_bits, bool bishop) {
    U64 occupancies[4096];
    U64 attacks[4096];
    U64 used_attacks[4096];

    U64 mask;
    if (bishop) {
        mask = generateBishopRelevantOccupancyMask(square);
    } else {
        mask = generateRookRelevantOccupancyMask(square);
    }

    int occupancy_ideces = 1 << relivant_bits;

    for (int i = 0; i < occupancy_ideces; ++i) {
        U64 occupancy = setOccupancy(i, relivant_bits, mask);
        occupancies[i] = occupancy;

        if (bishop) {
            attacks[i] = generateBishopAttackMask(square, occupancy);
        } else {
            attacks[i] = generateRookAttackMask(square, occupancy);
        }
    }

    while (true) {
        U64 magic = magic_number_candidate();
        if (magic == 0) continue; // Magic number cannot be zero

        memset(used_attacks, 0, sizeof(used_attacks));

        bool valid = true;
        for (int i = 0; i < occupancy_ideces; ++i) {
            int index = (occupancies[i] * magic) >> (64 - (bishop ? 9 : 12));
            if (used_attacks[index] != 0 && used_attacks[index] != attacks[i]) {
                valid = false;
                break;
            }
            used_attacks[index] = attacks[i];
        }

        if (valid) {
            return magic;
        }
        //printf("Invalid magic number: %llu\n", magic);
    }
}

void init_magic_numbers() {
    // Generate magic numbers for bishops
    for (int square = 0; square < 64; ++square) {
        int bits = bishop_relevant_occupancy_bits[square];
        U64 magic = magic_number(square, bits, true);
        std::cout << magic << ", " << std::endl;
    }

    std::cout << "\n\n";

    // Generate magic numbers for rooks
    for (int square = 0; square < 64; ++square) {
        int bits = rook_relevant_occupancy_bits[square];
        U64 magic = magic_number(square, bits, false);
        std::cout << magic << ", " << std::endl;
    }
}


void init_all() {
    generatePawnAttackMasks();
    generateKnightAttackMasks();
    generateKingAttackMasks();
    generateSliderAttackMasks();
}

inline U64 get_bishop_attacks(int square, U64 occupancy) {
    occupancy &= bishopPsudoAttackMasks[square];
    occupancy *= bishopMagicNumbers[square];
    occupancy >>= 64 - bishop_relevant_occupancy_bits[square]; // change to just 9 later?
    return bishopAttackMasks[square][occupancy];
}

inline U64 get_rook_attacks(int square, U64 occupancy) {
    occupancy &= rookPsudoAttackMasks[square];
    occupancy *= rookMagicNumbers[square];
    occupancy >>= 64 - rook_relevant_occupancy_bits[square]; // change to just 12 later?
    return rookAttackMasks[square][occupancy];
}

inline U64 get_queen_attacks(int square, U64 occupancy) {
    return get_bishop_attacks(square, occupancy) | get_rook_attacks(square, occupancy);
}

inline bool is_square_attacked(Board board, int square, bool side) {
    bool attacked = pawnAttackMasks[!side][square] & board.bitboards[p + 6*side];
    attacked |= knightAttackMasks[square] & board.bitboards[n + 6*side];
    if (attacked) return attacked;
    attacked = get_bishop_attacks(square, board.allPieces) & (board.bitboards[b + 6*side] | board.bitboards[q + 6*side]);
    if (attacked) return attacked;
    attacked = get_rook_attacks(square, board.allPieces) & (board.bitboards[r + 6*side] | board.bitboards[q + 6*side]);
    attacked |= kingAttackMasks[square] & board.bitboards[k + 6*side];
    return attacked;
}




void printAllAttackedSquares(Board board, bool side) {
    for (int row = 7; row >= 0; --row) {
        std::cout << row + 1 << "  ";
        for (int col = 0; col < 8; ++col) {
            int index = row * 8 + col;
            if (is_square_attacked(board, index, side)) {
                std::cout << "X ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\n   A B C D E F G H" << std::endl;
}

