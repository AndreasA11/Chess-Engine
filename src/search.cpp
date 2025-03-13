#include "search.hpp"
#include "engine.hpp"
#include "movement.hpp"

searchStruct Search::searchS = searchStruct();

void Search::searchPosition(int depth) {
    int score = Search::negamax(-50000, 50000, depth);
    std::cout << "bestmove: ";
    Movement::printMove(searchS.bestMove);
    std::cout << "\n";

}

int Search::negamax(int alpha, int beta, int depth) {
    if (depth == 0) {
        return Engine::evaluate();
    }
    //increment nodes count 
    searchS.nodes++;
    int tempBest;
    int oldAlpha = alpha;
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

    if(oldAlpha != alpha) {
        searchS.bestMove = tempBest; 
    }
    //move failes low
    return alpha;
}   