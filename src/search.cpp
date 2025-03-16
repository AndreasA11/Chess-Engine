#include "search.hpp"
#include "movement.hpp"
#include "algorithm"
searchStruct Search::searchS = searchStruct();


void Search::printMoveScores(Moves &moveList) {
	for(int count = 0 ; count < moveList.moveCount; count++) {
		std::cout << "    Move: ";
		Movement::printMove(moveList.moves[count]);
		std::cout << "    Score: " << scoreMove(moveList.moves[count]) << "\n";
        
    }
}

void Search::searchPosition(int depth) {
    int score = negamax(-50000, 50000, depth);
    std::cout << "info score cp " << score << " depth " << depth << " nodes " << searchS.nodes << " pv " << "\n";   
    for(int count = 0; count < searchS.pvLength[0]; ++count) {
        Movement::printMove(searchS.pvTable[0][count]);
        std::cout << " ";
    }
    std::cout << std::endl << "bestmove ";
    Movement::printMove(searchS.pvTable[0][0]);
    std::cout << "\n";
    
}



int Search::negamax(int alpha, int beta, int depth) {

    //init PV length
    searchS.pvLength[searchS.ply] = searchS.ply;

    if (depth == 0) {
        //run quiescence search
        return quiescence(alpha, beta);
    }
    //increment nodes count
    // std::cout << "nodes: " << searchS.nodes << "\n";  
    searchS.nodes++;

    int legalMoves = 0; 
    //see if the king tile is attacked(if we are in check)
    bool inCheck = BitBoard::isTileAttacked((BitBoard::bbState.side == White) 
                 ? BitBoard::getLSHBIndex(BitBoard::bbState.pieceBitboards[K]) 
                 : BitBoard::getLSHBIndex(BitBoard::bbState.pieceBitboards[k]), 
                 BitBoard::bbState.side ^ 1);
    
    //increase search depth if king is in check
    if(inCheck) {
        ++depth;
    }
    
    Moves moveList;
    Movement::generateMoves(moveList);
    
    sortMoves(moveList);

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
            
            //store killer moves
            if(getMoveCapture(moveList.moves[count]) == 0) {
                searchS.killerMoves[1][searchS.ply] = searchS.killerMoves[0][searchS.ply];
                searchS.killerMoves[0][searchS.ply] = moveList.moves[count];
            }
            
            //move fails high
            return beta;
        }
        if(score > alpha) {

            //store history moves
            if(getMoveCapture(moveList.moves[count]) == 0) {
                searchS.historyMoves[getMovePiece(moveList.moves[count])][getMoveTarget(moveList.moves[count])] += depth;
            }
            alpha = score;

            searchS.pvTable[searchS.ply][searchS.ply] = moveList.moves[count];
            
            for(int nextPly = searchS.ply + 1; nextPly < searchS.pvLength[searchS.ply + 1]; ++nextPly) {
                //copt move from deeper ply into current ply's line
                searchS.pvTable[searchS.ply][nextPly] = searchS.pvTable[searchS.ply + 1][nextPly];
            }
            searchS.pvLength[searchS.ply] = searchS.pvLength[searchS.ply + 1];
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
    //move failes low
    return alpha;
}   


int Search::quiescence(int alpha, int beta) {
    searchS.nodes++;
    int evaluate = Search::evaluate();
    //fail hard beta cutoff
    if(evaluate >= beta) {
        //move fails high
        return beta;
    }
    if(evaluate > alpha) {
        alpha = evaluate;
    }
    
    Moves moveList;
    Movement::generateMoves(moveList);
    sortMoves(moveList);
    for(int count = 0; count < moveList.moveCount; ++count) {
        //preserve board state
        copy_board();
        searchS.ply++;
        if(Movement::makeMove(moveList.moves[count], onlyCaptures) == 0) {
            searchS.ply--;
            continue;
        }
        //score current move
        int score = -quiescence(-beta, -alpha);
        --searchS.ply;
        take_back();
        
        //fail hard beta cutoff
        if(score >= beta) {
            //move fails high
            return beta;
        }
        if(score > alpha) {
            alpha = score;
        }
    }
    return alpha;
}

int Search::evaluate() {
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


int Search::scoreMove(int move) {
    
    //score capture move
    if(getMoveCapture(move)) {
        int targetPiece = P;

        //pick up bitboard piece index ranges depending on side
        int startPiece;
        int endPiece;
        if(BitBoard::bbState.side == White) { //white to move
            startPiece = p;
            endPiece = k;
        } else { //black to move
            startPiece = P;
            endPiece = K;
        }
        //loop over opposite colored bitboards
        for(int bbPiece = startPiece; bbPiece <= endPiece; ++bbPiece) {
            if(getBit(BitBoard::bbState.pieceBitboards[bbPiece], getMoveTarget(move))) {
                //if piece on target tile we remove from that bitboard
                targetPiece = bbPiece;
                break;
            }
        }
        //score move with mvv lva table
        return mvvLVA[getMovePiece(move)][targetPiece];
    } else {
        //score quiet move
        //score 1st killer move
        if(searchS.killerMoves[0][searchS.ply] == move) {
            return 9000;
        }
        //score 2nd killer move
        else if(searchS.killerMoves[1][searchS.ply] == move) {
            return 8000;
        }
        //score history move
        else {
            return searchS.historyMoves[getMovePiece(move)][getMoveTarget(move)];
        }

    }
    return 0;
    
}

//score moves in descending order
int Search::sortMoves(Moves &moveList) {
    std::cout << std::endl;
    int moveScores[moveList.moveCount];

    for (int count = 0; count < moveList.moveCount; count++) {
        moveScores[count] = scoreMove(moveList.moves[count]);
    }   
        

    for(int cur = 0; cur < moveList.moveCount; ++cur) {
        for(int next = cur + 1; next < moveList.moveCount; ++next) {
            if(moveScores[cur] < moveScores[next]) {
                //swap score
                int tempScore = moveScores[cur];
                moveScores[cur] = moveScores[next];
                moveScores[next] = tempScore;
                //swap move
                int tempMove = moveList.moves[cur];
                moveList.moves[cur] = moveList.moves[next];
                moveList.moves[next] = tempMove;
            }
        }
    }
    return 0;
}

