#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdint.h>
#include <iostream> 


//move struct: list of moves, along with counter on which part of list we are at
struct Moves{
    //moves
    int moves[256];
    //move count
    int moveCount = 0;
};

namespace Movement {
    extern Moves moveStruct;

    //add a move to the moves list, update move counter
    inline void addMove(int move) {  
        moveStruct.moves[moveStruct.moveCount] = move;
        ++moveStruct.moveCount;
    }


    //generate all moves
    void generateMoves();

    //generate all moves helper functions
    //pawn moves
    void bitPawnMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    //castling moves
    void bitCastling(int piece);
    //knight moves
    void bitKnightMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    //bishop moves
    void bitBishopMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    //rook moves
    void bitRookMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    //queen moves
    void bitQueenMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    //king moves (non castling)
    void bitKingMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);

    //print the moves in the move list
    void printMove(int move);

    //print move list for debugging purposes (more information than printMove)
    void printMoveList(); 

    int makeMove(int move, int moveFlag);
};


#endif //MOVEMENT_H