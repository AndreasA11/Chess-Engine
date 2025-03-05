#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdint.h> // for uint

struct Moves{
    //moves
    int moves[256];
    //move count
    int moveCount = 0;

};

namespace Movement {
    extern Moves moveStruct;

    inline void addMove(int move) {  
        moveStruct.moves[moveStruct.moveCount] = move;
        ++moveStruct.moveCount;
    }


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


    void printMove(int move);

    void printMoveList(); //for debugging
};


#endif //MOVEMENT_H