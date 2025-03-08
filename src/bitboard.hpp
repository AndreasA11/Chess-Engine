#ifndef BITBOARD_H
#define BITBOARD_H

#include "consts.hpp"
#include "magicNumbers.hpp"
#include <algorithm>



namespace BitBoard {
    
    //state of the bitboard, variables that need to be accessed and changed in outside files
    struct BitBoardState {
        //board state variables
        //side
        int side = White;
        //en passant
        int enpassant = noTile;
        //castling rights
        int canCastle = 0;
        //bitboard for all 12 pieces
        uint64_t pieceBitboards[12];
        //occupancies for white, black, and both colored pieces
        uint64_t occupancies[3];

        //attack tables
        uint64_t pawnAttack[2][64]; //[color][tile]
        uint64_t knightAttack[64];
        uint64_t kingAttack[64]; 
        uint64_t bishopMasks[64]; //bishop masks
        uint64_t rookMasks[64]; //rook masks
        uint64_t bishopAttacks[64][512]; //lookip table for bishops
        uint64_t rookAttacks[64][4096]; //lookup table for rooks
    };    
    
    //bitboard state
    extern BitBoardState bbState; 
    //magic numbers for bishop and rook
    static MagicNumbers m;
    
    //bitboard masking functions for leaping piecs P, N, K
    uint64_t maskPawnAttacks(int tile, Color colorIn); //pawn attacks on a given tile and color
    uint64_t maskKnightAttacks(int tile); //knight attacks on a given tile
    uint64_t maskKingAttacks(int tile); //king attacks on a given tile
    
    //bitboard masking functions for get sliding piece attacks B, R
    uint64_t maskBishopAttacks(int tile);
    uint64_t maskRookAttacks(int tile);
    uint64_t BishopOnFly(int tile, uint64_t block);
    uint64_t RookOnFly(int tile, uint64_t block);

    //get sliding pieces attacks
    //get bishop attacks
    static inline uint64_t getBishopAttacks(int tile, uint64_t occupancy) {
        //get bishop attacks
        occupancy &= bbState.bishopMasks[tile];
        occupancy *= m.bishopMagicNumbers[tile];
        occupancy >>= 64 - bishopRelevantBits[tile];
        return bbState.bishopAttacks[tile][occupancy];
    }
    //get rook attacks
    static inline uint64_t getRookAttacks(int tile, uint64_t occupancy) {
        //get rook attacks
        occupancy &= bbState.rookMasks[tile];
        occupancy *= m.rookMagicNumbers[tile];
        occupancy >>= 64 - rookRelevantBits[tile];
        return bbState.rookAttacks[tile][occupancy];
    }
    //getQueenAttacks
    static inline uint64_t getQueenAttacks(int tile, uint64_t occupancy) {
        uint64_t queenAttacks = ZERO;
        queenAttacks |= getBishopAttacks(tile, occupancy);
        queenAttacks |= getRookAttacks(tile, occupancy);
        return queenAttacks;
    }
    

    //init attacks
    void initLeaperAttacks(); //leaper pieces(p, n, k)
    void initSliderAttacks(RB pieceIn); //sliding pieces(b, r, q)

    //is current square attacked by the current given side
    bool isTileAttacked(int tile, int side);

    //print bitboard
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

    //set occupancy bitboard
    uint64_t setOccupancy(int index, int bitsInMask, uint64_t attackMask);
    
    //print board
    void printBoard();
    //print all tiles that are attacked
    void printAttackedSquares(int side);
    

    
};



#endif //BITBOARD_H