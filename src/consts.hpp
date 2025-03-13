#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h> // for uint
#include <string>

/***********************
  --------------------
    CONSTANTS BEGIN
  --------------------
***********************/

const uint64_t ZERO = 0;
const uint64_t ONE = 1;

enum Color { //the color of the pieces, used for occupancies
    White, Black, Both
};

enum RB { //rook or bishop
    rook, bishop
};

enum Piece { //capital pieces are white, lowercase pieces are black
    P, N, B, R, Q, K, p, n, b, r, q, k
};

enum CastlePerm { //bit indications of which types of castling(if any) are available
    WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8
};

enum moveTypes {
    allMoves, onlyCaptures
};

enum tiles { //chess board 
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, noTile,
};

const std::string tileToCoord[64] = { //string chess board
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};

//unicode chess pieces
const std::string unicodePieces[12] = {"♙", "♘", "♗", "♖", "♕", "♔", "♟", "♞", "♝", "♜", "♛", "♚"};
//convert ASCII character pieces to encoded constants
const int charPieces[115] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k
};

const char promotedPieces[] = {
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n',
};

const int materialScore[12] = {
    100, //P
    300, //N
    350, //B
    500, //R
    1000, //Q
    10000, //K
    -100, //p
    -300, //n
    -350, //b
    -500, //r
    -1000, //q
    -10000  //k
};

//consts for files 
const uint64_t notAfile = 18374403900871474942ULL; // the entire A file is 0's the rest is 1's
const uint64_t notHfile = 9187201950435737471ULL; // the entire H file is 0's the rest is 1's
const uint64_t notHGfile = 4557430888798830399ULL; // the entire A and B file is 0's the rest is 1's
const uint64_t notABfile = 18229723555195321596ULL; // the entire H and G file is 0's the rest is 1's


//the number of relevant bits (not including edges since they cannot block anything) for masking for rook
const int rookRelevantBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

//the number of relevant bits (not including edges since they cannot block anything) for masking for bishop
const int bishopRelevantBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
}; 

const int castlingRights[64] = {
    7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

// pawn positional score
const int pawnScore[64] = 
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  25,  25,  10,  10,  10,
     5,   5,  10,  25,  25,   5,   5,   5,
     1,   0,   0,   5,   5,   0,   0,   1,
     0,  -1,   0, -10, -10,   0,  -1,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knightScore[64] = 
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
const int bishopScore[64] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,  10,  10,   0,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  20,  20,  20,  20,   0,   0,
     0,  10,   5,  10,  10,   5,  10,   0,
     0,  20,   0,   0,   0,   0,  20,   0,
     0,   0, -10,   0,   0, -10,   0,   0

};

// rook positional score
const int rookScore[64] =
{
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     10,  0,  10,  20,  20,  10,   0,  10,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0

};

// king positional score
const int kingScore[64] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5,  -5,  -5,   0,   5,   0,
     0,   0,  15,  -5,  -15,  0,  20,   0
};

// mirror positional score tables for opposite side
const int mirrorScore[128] =
{
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};


/***********************
  --------------------
      CONSTANTS END
  --------------------
***********************/

/***********************
  --------------------
      TESTING FENS
  --------------------
***********************/

const std::string empty_board = "8/8/8/8/8/8/8/8 w - - ";
const std::string start_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";
const std::string tricky_position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
const std::string killer_position = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
const std::string cmk_position = "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";
const std::string position6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ";

/***********************
  --------------------
      TESTING FENS
  --------------------
***********************/

/***********************
  --------------------
      MACROS BEGIN
  --------------------
***********************/

#define setBitOn(bitBoardIn, tile) ((bitBoardIn) |= (ONE << tile))
#define getBit(bitBoardIn, tile) ((bitBoardIn) & (ONE << tile))
#define popBit(bitBoardIn, tile) ((bitBoardIn) &= ~(ONE << tile))

//binary move bits
    /*
        sourceTile: 0x3f
        targetTile: 0xfc0
        piece: 0xf000
        promoted piece: 0xf0000
        capture: 0x100000
        double push: 0x200000
        en passant: 0x400000
        castling: 0x800000
    */

#define encodeMove(source, target, piece, promo, cap, doub, enpass, castling) (source) | (target << 6) | (piece << 12) | (promo << 16) | (cap << 20) | (doub << 21)| (enpass << 22) | (castling << 23)
#define getMoveSource(move) (move & 0x3f) //extract source tile
#define getMoveTarget(move) ((move & 0xfc0) >> 6) //extract target tile
#define getMovePiece(move) ((move & 0xf000) >> 12) //see what piece is moving
#define getMovePromo(move) ((move & 0xf0000) >> 16) //see what piece(if any) is promoted
#define getMoveCapture(move) (move & 0x100000) //see if capture flag is on or not
#define getDoublePPush(move) (move & 0x200000)
#define getEnPassant(move) (move & 0x400000)
#define getCastling(move) (move & 0x800000)

#define copy_board()   \
    uint64_t bitboards_copy[12], occupancies_copy[3];    \
    int side_copy, enpassant_copy, castle_copy;   \
    memcpy(bitboards_copy, BitBoard::bbState.pieceBitboards, 96);  \
    memcpy(occupancies_copy, BitBoard::bbState.occupancies, 24); \
    side_copy = BitBoard::bbState.side; \
    enpassant_copy = BitBoard::bbState.enpassant; \
    castle_copy = BitBoard::bbState.canCastle;   \

// restore board state
#define take_back()                                                \
    memcpy(BitBoard::bbState.pieceBitboards, bitboards_copy, 96);  \
    memcpy(BitBoard::bbState.occupancies, occupancies_copy, 24);   \
    BitBoard::bbState.side = side_copy, BitBoard::bbState.enpassant = enpassant_copy, BitBoard::bbState.canCastle = castle_copy;   \



/***********************
  --------------------
       MACROS END
  --------------------
***********************/




#endif //CONSTANTS_H