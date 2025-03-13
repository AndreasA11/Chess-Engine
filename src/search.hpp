#include "movement.hpp"
#include "bitboard.hpp"


struct searchStruct {
    int ply; //half move
    int bestMove;
    long nodes;
};

namespace Search {
    extern searchStruct searchS;
    void searchPosition(int depth);
    int negamax(int alpha, int beta, int depth);
};