#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h> // for uint
#include <iostream>
#include <string>
#include "magicNumbers.hpp"

const uint64_t ZERO = 0;
const uint64_t ONE = 1;

#define setBitOn(bitBoardIn, tile) ((bitBoardIn) |= (ONE << tile))
#define getBit(bitBoardIn, tile) ((bitBoardIn) & (ONE << tile))
#define popBit(bitBoardIn, tile) ((bitBoardIn) &= ~(ONE << tile))

enum Color {
    White, Black, Both
};

enum RB {
    rook, bishop
};

enum Piece {
    P, N, B, R, Q, K, p, n, b, r, q, k
};

enum CastlePerm {
    WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8
};

enum tiles {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, noTile,
};

const std::string tileToCoord[64] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};

//unicode pieces
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

struct BitBoard {
    //consts for files
    const uint64_t notAfile = 18374403900871474942ULL; // the entire A file is 0's the rest is 1's
    const uint64_t notHfile = 9187201950435737471ULL; // the entire H file is 0's the rest is 1's
    const uint64_t notHGfile = 4557430888798830399ULL; // the entire A and B file is 0's the rest is 1's
    const uint64_t notABfile = 18229723555195321596ULL; // the entire H and G file is 0's the rest is 1's
    
    //bitboard for all 12 pieces
    uint64_t pieceBitboards[12];
    //occupancies for white, black, and both colored pieces
    uint64_t occupancies[3];
    
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
    
    //side to move
    int side = White;
    //en passant
    int enpassant = noTile;
    //castling rights
    int canCastle = 0;
    
    //magic numbers for bishop and rook
    MagicNumbers m;

    //attack tables
    uint64_t pawnAttack[2][64]; //[color][tile]
    uint64_t knightAttack[64];
    uint64_t kingAttack[64]; 
    uint64_t bishopMasks[64]; //bishop masks
    uint64_t rookMasks[64]; //rook masks
    uint64_t bishopAttacks[64][512]; //lookip table for bishops
    uint64_t rookAttacks[64][4096]; //lookup table for rooks
    
    //bitboard functions
    uint64_t maskPawnAttacks(int tile, Color colorIn); //pawn attacks on a given tile and color
    uint64_t maskKnightAttacks(int tile); //knight attacks on a given tile
    uint64_t maskKingAttacks(int tile); //king attacks on a given tile
    
    //helpers for get sliding piece attacks
    uint64_t maskBishopAttacks(int tile);
    uint64_t maskRookAttacks(int tile);
    uint64_t BishopOnFly(int tile, uint64_t block);
    uint64_t RookOnFly(int tile, uint64_t block);

    //is current square attacked by the current given side
    bool isTileAttacked(int tile, int side);

    //get sliding pieces attacks
    //get bishop attacks
    inline uint64_t getBishopAttacks(int tile, uint64_t occupancy) {
        //get bishop attacks
        occupancy &= bishopMasks[tile];
        occupancy *= m.bishopMagicNumbers[tile];
        occupancy >>= 64 - bishopRelevantBits[tile];
        return bishopAttacks[tile][occupancy];
    }
    //get rook attacks
    inline uint64_t getRookAttacks(int tile, uint64_t occupancy) {
        //get rook attacks
        occupancy &= rookMasks[tile];
        occupancy *= m.rookMagicNumbers[tile];
        occupancy >>= 64 - rookRelevantBits[tile];
        return rookAttacks[tile][occupancy];
    }
    //getQueenAttacks
    inline uint64_t getQueenAttacks(int tile, uint64_t occupancy) {
        uint64_t queenAttacks = ZERO;
        queenAttacks |= getBishopAttacks(tile, occupancy);
        queenAttacks |= getRookAttacks(tile, occupancy);
        return queenAttacks;
    }
    

    //init attacks
    void initLeaperAttacks();
    void initSliderAttacks(RB pieceIn);

    //generate all moves
    void generateMoves();

    //generate all moves helper functions
    void bitPawnMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    void bitCastling(int piece);
    void bitKnightMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    void bitBishopMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    void bitRookMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    void bitQueenMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    void bitKingMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    
    //helper functions
    void printBitBoard(uint64_t bitBoardIn);

    //count bits within a bitboard
    static inline int countBitsOn(uint64_t bitboardIn) {
        int count = 0;
        //keep resetting least signigicant 1st bit
        while(bitboardIn) { 
            ++count;
            //reset
            bitboardIn &= bitboardIn - 1;
        }
        return count;
    }

    //return index of least significant high bit
    static inline int getLSHBIndex(uint64_t bitboardIn) {
        
        if(bitboardIn) {
            //count trailing bits before LSHB (least sig high bit)
            return countBitsOn((bitboardIn & -bitboardIn) - 1);
        } else {
            return -1; //illegal index
        }
    }

    uint64_t setOccupancy(int index, int bitsInMask, uint64_t attackMask);
    
    void printBoard();
    void printAttackedSquares(int side);
    //parse FEN 
    void parseFEN(std::string fen);
        
};


#endif //BITBOARD_H