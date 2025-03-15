#include "search.hpp"
#include "engine.hpp"
#include "movement.hpp"

searchStruct Search::searchS = searchStruct();

void Search::searchPosition(int depth) {
    int score = Search::negamax(-50000, 50000, depth);
    if(searchS.bestMove) {
        std::cout << "info score cp " << score << " depth " << depth << " nodes " << searchS.nodes << "\n";   
        std::cout << "bestmove: ";
        Movement::printMove(searchS.bestMove);
        std::cout << "\n";
    }
    

}

int Search::negamax(int alpha, int beta, int depth) {
    if (depth == 0) {
        return Engine::evaluate();
    }
    //increment nodes count 
    searchS.nodes++;
    int tempBest;
    int oldAlpha = alpha;

    int legalMoves = 0; 
    //see if the king tile is attacked(if we are in check)
    bool inCheck = BitBoard::isTileAttacked((BitBoard::bbState.side == White) 
                 ? BitBoard::getLSHBIndex(BitBoard::bbState.pieceBitboards[K]) 
                 : BitBoard::getLSHBIndex(BitBoard::bbState.pieceBitboards[k]), 
                 BitBoard::bbState.side ^ 1);
    
    Moves moveList;
    Movement::generateMoves(moveList);
    for(int count = 0; count < moveList.moveCount; ++count) {
        //preserve board state
        copy_board();
        searchS.ply++;
        if(Movement::makeMove(moveList.moves[count], allMoves) == 0) {
            searchS.ply--;
            continue;
        }
        ++legalMoves;
        //score current move
        int score = -negamax(-beta, -alpha, depth - 1);
        --searchS.ply;
        take_back();
        
        //fail hard beta cutoff
        if(score >= beta) {
            //move fails high
            return beta;
        }
        if(score > alpha) {
            alpha = score;
            if(searchS.ply == 0) {
                tempBest = moveList.moves[count];
            }
        }

    }
    //no legal moves to make
    if(legalMoves == 0) {
        if(inCheck) {
            //return mating score
            return -49000 + searchS.ply;
        } else {
            return 0; //stalemate
        }
    }
    if(oldAlpha != alpha) {
        searchS.bestMove = tempBest; 
    }
    //move failes low
    return alpha;
}   