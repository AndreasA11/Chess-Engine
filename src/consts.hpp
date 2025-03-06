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


/***********************
  --------------------
      CONSTANTS END
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

/***********************
  --------------------
       MACROS END
  --------------------
***********************/




#endif //CONSTANTS_H