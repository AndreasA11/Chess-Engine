#include "engine.hpp"
#include <chrono>
#include "consts.hpp"
#include "bitboard.hpp"


std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> Engine::getTimeInMS() {
    auto start = std::chrono::high_resolution_clock::now();
    return start;
}

int Engine::getTimeInterval(
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> begin, 
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> end) {
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    return duration;
}

int Engine::evaluate() {
    int score = 0;

    uint64_t bitboardCopy; 
    int piece;
    int tile;

    for(int bbPiece = P; bbPiece <= k; ++bbPiece) {
        bitboardCopy = BitBoard::bbState.pieceBitboards[bbPiece];
        while(bitboardCopy) {
            piece = bbPiece;
            tile = BitBoard::getLSHBIndex(bitboardCopy);
            
            //score material weights
            score += materialScore[bbPiece];
            
            //score positional piece scores
            switch (piece) {
                case P:
                    score += pawnScore[tile];
                    break;
                case N:
                    score += knightScore[tile];
                    break;
                case B:
                    score += bishopScore[tile];
                    break;
                case R:
                    score += rookScore[tile];
                    break;
                case Q:
                    break;
                case K:
                    score += kingScore[tile];
                    break;

                case p:
                    score -= pawnScore[mirrorScore[tile]];
                    break;
                case n:
                    score -= knightScore[mirrorScore[tile]];
                    break;
                case b:
                    score -= bishopScore[mirrorScore[tile]];
                    break;
                case r:
                    score -= rookScore[mirrorScore[tile]];
                    break;
                case q:
                    break;
                case k:
                    score -= kingScore[mirrorScore[tile]];
                    break;
            }
            popBit(bitboardCopy, tile);
        }
    }

    if(BitBoard::bbState.side == White) {
        return score;
    } else {
        return -score;
    }
    
}
