#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdint.h>
#include <iostream> 
#include "consts.hpp"
#include "bitboard.hpp"

//move struct: list of moves, along with counter on which part of list we are at
struct Moves{
    //moves
    int moves[256];
    //move count
    int moveCount = 0;
};

namespace Movement {
    

    inline void addMove(Moves &moveList, int move) {
        moveList.moves[moveList.moveCount] = move;
        ++moveList.moveCount;
    }

    //generate all moves
    void generateMoves(Moves &moveList);

    

    //generate all moves helper functions
    //pawn moves
    void bitPawnMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    //castling moves
    void bitCastling(Moves &moveList, int piece);
    //knight moves
    void bitKnightMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    //bishop moves
    void bitBishopMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    //rook moves
    void bitRookMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    //queen moves
    void bitQueenMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);
    //king moves (non castling)
    void bitKingMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks);

    //print the moves in the move list
    void printMove(int move);

    //print move list for debugging purposes (more information than printMove)
    void printMoveList(Moves &moveList); 

    int makeMove(int move, int moveFlag);

    
};


#endif //MOVEMENT_H