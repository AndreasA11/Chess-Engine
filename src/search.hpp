#include "movement.hpp"
#include "bitboard.hpp"


struct searchStruct {
    int ply; //half move
    long nodes;
    int killerMoves[2][64];
    int historyMoves[12][64];
    int pvLength[64];
    int pvTable[64][64];
};

namespace Search {
    extern searchStruct searchS;
    void searchPosition(int depth);
    int evaluate();
    int negamax(int alpha, int beta, int depth);
    int quiescence(int alpha, int beta);
    int scoreMove(int move);
    int sortMoves(Moves &moveList); //in descending order
    void printMoveScores(Moves &moveList);
};