#include "movement.hpp"
#include "bitboard.hpp"

// Moves Movement::moveStruct = Moves();

void Movement::printMove(int move) {
    if(getMovePromo(move)) {
        std::cout << tileToCoord[getMoveSource(move)] 
            << tileToCoord[getMoveTarget(move)]
            << promotedPieces[getMovePromo(move)] << "\n"; 
    } else {
        std::cout << tileToCoord[getMoveSource(move)] 
            << tileToCoord[getMoveTarget(move)] << "\n";
    }
    
}

void Movement::printMoveList(Moves &moveList) { 
    std::cout << "\n    move   piece  capture   double   enpassant   castling\n\n";

    if(!moveList.moveCount) {
        std::cout << "    No move in move list!\n";
        return;
    }

    for(int count = 0; count < moveList.moveCount; ++count) {
        int move = moveList.moves[count];

        //print move
        std::cout << "    " << tileToCoord[getMoveSource(move)] 
            << tileToCoord[getMoveTarget(move)] << "  "
            << (getMovePromo(move) ? promotedPieces[getMovePromo(move)] : ' ')
            << unicodePieces[getMovePiece(move)] << "         "
            << (getMoveCapture(move) ? 1 : 0) << "         " 
            << (getDoublePPush(move) ? 1 : 0) << "        "
            << (getEnPassant(move) ? 1 : 0) << "           "
            << (getCastling(move) ? 1 : 0) << "\n";     
        
    }    
    std::cout << "\n    Total number of moves: " << moveList.moveCount << "\n";
}

void Movement::generateMoves(Moves &moveList) {
    moveList.moveCount = 0;
    int sourceTile = 0;
    int targetTile = 0;
    //define current piece bitboard copy and attacks
    uint64_t bitboard = ZERO;
    uint64_t attacks = ZERO;
    for(int piece = P; piece <= k; ++piece) {
        bitboard = BitBoard::bbState.pieceBitboards[piece];
        //white pawn moves, white king castle moves
        if(BitBoard::bbState.side == White) {
            //white pawn moves 
            bitPawnMoves(moveList, piece, bitboard, sourceTile, targetTile, attacks);
            //white castling moves
            bitCastling(moveList, piece);
        } else { //black pawn moves, black king castle moves
            //black pawn moves
            bitPawnMoves(moveList, piece, bitboard, sourceTile, targetTile, attacks);
            //black castling moves
            bitCastling(moveList, piece);
        }
        //knight moves
        bitKnightMoves(moveList, piece, bitboard, sourceTile, targetTile, attacks);
        //bishop moves
        bitBishopMoves(moveList, piece, bitboard, sourceTile, targetTile, attacks);
        //rook moves
        bitRookMoves(moveList, piece, bitboard, sourceTile, targetTile, attacks);
        //queen moves
        bitQueenMoves(moveList, piece, bitboard, sourceTile, targetTile, attacks);
        //king moves (non castle)
        bitKingMoves(moveList, piece, bitboard, sourceTile, targetTile, attacks);
    }
}

void Movement::bitPawnMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if(piece == P && BitBoard::bbState.side == White) {
        //loop over white pawns in white pawn bitboard
        while(bitboard) {
            sourceTile = BitBoard::getLSHBIndex(bitboard);
            targetTile = sourceTile - 8;
            //generate quiet pawn moves
            if(!(targetTile < a8) && !getBit(BitBoard::bbState.occupancies[Both], targetTile)) {
                //pawn promo
                if(sourceTile >= a7 && sourceTile <= h7) {
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, Q, 0, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, R, 0, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, B, 0, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, N, 0, 0, 0, 0));
                } else { //pawn pushes
                    // one square pawn push
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 0, 0, 0, 0));
                    if((sourceTile >= a2 && sourceTile <= h2) && !getBit(BitBoard::bbState.occupancies[Both], (targetTile - 8))) {
                        //two square pawn push
                        addMove(moveList, encodeMove(sourceTile, (targetTile - 8), piece, 0, 0, 1, 0, 0));
                    }
                }
            }
            //init white pawn attacks bitboard
            attacks = BitBoard::bbState.pawnAttack[BitBoard::bbState.side][sourceTile] & BitBoard::bbState.occupancies[Black];
            while(attacks) {
                targetTile = BitBoard::getLSHBIndex(attacks);
                if(sourceTile >= a7 && sourceTile <= h7) {
                    //promotion pawn capture
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, Q, 1, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, R, 1, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, B, 1, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, N, 1, 0, 0, 0));
                } else {  
                    //normal pawn caputre    
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 1, 0, 0, 0));
                }
                popBit(attacks, targetTile);
            }
            //generate enpassant captures
            if(BitBoard::bbState.enpassant != noTile) {
                uint64_t enpassantAttacks = BitBoard::bbState.pawnAttack[BitBoard::bbState.side][sourceTile] & (ONE << BitBoard::bbState.enpassant);
                //check if enpassant capture is available
                if(enpassantAttacks) {
                    int enpassantTargetTile = BitBoard::getLSHBIndex(enpassantAttacks);
                    addMove(moveList, encodeMove(sourceTile, enpassantTargetTile, piece, 0, 1, 0, 1, 0));
                }
            }
            //pop LSHB from bitboard
            popBit(bitboard, sourceTile);
        }
    } else if(piece == p && BitBoard::bbState.side == Black) {
        //loop over black pawns in black pawn bitboard
        while(bitboard) {
            sourceTile = BitBoard::getLSHBIndex(bitboard);
            targetTile = sourceTile + 8;
            //generate quiet pawn moves
            if(!(targetTile > h1) && !getBit(BitBoard::bbState.occupancies[Both], targetTile)) {
                //pawn promo
                if(sourceTile >= a2 && sourceTile <= h2) {
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, q, 0, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, r, 0, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, b, 0, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, n, 0, 0, 0, 0));
                } else { //pawn pushes
                    // one square pawn push
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 0, 0, 0, 0));
                    //two square pawn push
                    if((sourceTile >= a7 && sourceTile <= h7) && !getBit(BitBoard::bbState.occupancies[Both], (targetTile + 8))) {
                        addMove(moveList, encodeMove(sourceTile, (targetTile + 8), piece, 0, 0, 1, 0, 0));
                    }
                }
            }
            //init black pawn attacks bitboard
            attacks = BitBoard::bbState.pawnAttack[BitBoard::bbState.side][sourceTile] & BitBoard::bbState.occupancies[White];
            while(attacks) {
                targetTile = BitBoard::getLSHBIndex(attacks);
                if(sourceTile >= a2 && sourceTile <= h2) {
                    //promotion pawn capture
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, q, 1, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, r, 1, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, b, 1, 0, 0, 0));
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, n, 1, 0, 0, 0));
                } else { 
                    // normal pawn capture 
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 1, 0, 0, 0));
                }
                popBit(attacks, targetTile);
            }
            //generate enpassant captures
            if(BitBoard::bbState.enpassant != noTile) {
                uint64_t enpassantAttacks = BitBoard::bbState.pawnAttack[BitBoard::bbState.side][sourceTile] & (ONE << BitBoard::bbState.enpassant);
                //check if enpassant capture is available
                if(enpassantAttacks) {
                    int enpassantTargetTile = BitBoard::getLSHBIndex(enpassantAttacks);
                    addMove(moveList, encodeMove(sourceTile, enpassantTargetTile, piece, 0, 1, 0, 1, 0));
                }
            }
            //pop LSHB from bitboard
            popBit(bitboard, sourceTile);
        }
    }
}

void Movement::bitCastling(Moves&moveList, int piece) {
    if(piece == K && BitBoard::bbState.side == White) {
        //king side castling
        if(BitBoard::bbState.canCastle & WKCA) {
            //make sure squares between king and king side rook are free
            if(!getBit(BitBoard::bbState.occupancies[Both], f1) && !getBit(BitBoard::bbState.occupancies[Both], g1)) {
                //make sure king and the f1 tile are not targeted by enemy pieces 
                if(!BitBoard::isTileAttacked(e1, Black) && !BitBoard::isTileAttacked(f1, Black)) {
                    addMove(moveList, encodeMove(e1, g1, piece, 0, 0, 0, 0, 1));
                }
            }
        } //canCastle 
        //queen side castling
        if(BitBoard::bbState.canCastle & WQCA) {
            //make sure squares between king and queen side rook are free
            if(!getBit(BitBoard::bbState.occupancies[Both], d1) && !getBit(BitBoard::bbState.occupancies[Both], c1) && !getBit(BitBoard::bbState.occupancies[Both], b1)) {
                //make sure king and the d1 tile are not targeted by enemy pieces 
                if(!BitBoard::isTileAttacked(e1, Black) && !BitBoard::isTileAttacked(d1, Black)) {
                    addMove(moveList, encodeMove(e1, c1, piece, 0, 0, 0, 0, 1));
                }
            }
        }
    } else {
        if(piece == k && BitBoard::bbState.side == Black) {
            //king side castling
            if(BitBoard::bbState.canCastle & BKCA) {
                //make sure squares between king and king side rook are free
                if(!getBit(BitBoard::bbState.occupancies[Both], f8) && !getBit(BitBoard::bbState.occupancies[Both], g8)) {
                    //make sure king and the f8 tile are not targeted by enemy pieces 
                    if(!BitBoard::isTileAttacked(e8, White) && !BitBoard::isTileAttacked(f8, White)) {
                        addMove(moveList, encodeMove(e8, g8, piece, 0, 0, 0, 0, 1));
                    }
                }
            }
            //queen side castling
            if(BitBoard::bbState.canCastle & BQCA) {
                //make sure squares between king and queen side rook are free
                if(!getBit(BitBoard::bbState.occupancies[Both], d8) && !getBit(BitBoard::bbState.occupancies[Both], c8) && !getBit(BitBoard::bbState.occupancies[Both], b8)) {
                    //make sure king and the d8 tile are not targeted by enemy pieces 
                    if(!BitBoard::isTileAttacked(e8, White) && !BitBoard::isTileAttacked(d8, White)) {
                        addMove(moveList, encodeMove(e8, c8, piece, 0, 0, 0, 0, 1));
                    }
                }
            }
        }
    }
}

void Movement::bitKnightMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if((BitBoard::bbState.side == White) ? piece == N : piece == n) {
        //loop over source tiles of piece bitboard copy
        while(bitboard) {
            sourceTile = BitBoard::getLSHBIndex(bitboard);
            //init piece attacks in order to get set of target squares, make sure not to attack same colored pieces
            attacks = BitBoard::bbState.knightAttack[sourceTile] & ((BitBoard::bbState.side == White) ? ~BitBoard::bbState.occupancies[White] : ~BitBoard::bbState.occupancies[Black]);
            while(attacks) { //loop over tiles in attacks
                targetTile = BitBoard::getLSHBIndex(attacks);
                //quiet moves
                if(!getBit(((BitBoard::bbState.side == White) ? BitBoard::bbState.occupancies[Black] : BitBoard::bbState.occupancies[White]), targetTile)) {
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 0, 0, 0, 0));
                } else {
                    //capture moves
                addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 1, 0, 0, 0));
                }
                popBit(attacks, targetTile);
            }
            //pop LSHB
            popBit(bitboard, sourceTile);
        }
    }
}

void Movement::bitBishopMoves(Moves &moveList,int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if((BitBoard::bbState.side == White) ? piece == B : piece == b) {
        //loop over source tiles of piece bitboard copy
        while(bitboard) {
            sourceTile = BitBoard::getLSHBIndex(bitboard);
            //init piece attacks in order to get set of target squares, make sure not to attack same colored pieces
            attacks = BitBoard::getBishopAttacks(sourceTile, BitBoard::bbState.occupancies[Both]) & ((BitBoard::bbState.side == White) ? ~BitBoard::bbState.occupancies[White] : ~BitBoard::bbState.occupancies[Black]);
            while(attacks) { //loop over tiles in attacks
                targetTile = BitBoard::getLSHBIndex(attacks);
                //quiet moves
                if(!getBit(((BitBoard::bbState.side == White) ? BitBoard::bbState.occupancies[Black] : BitBoard::bbState.occupancies[White]), targetTile)) {          
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 0, 0, 0, 0));
                } else {
                    //capture moves
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 1, 0, 0, 0));
                }
                popBit(attacks, targetTile);
            }
            //pop LSHB
            popBit(bitboard, sourceTile);
        }
    }
}

void Movement::bitRookMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if((BitBoard::bbState.side == White) ? piece == R : piece == r) {
        //loop over source tiles of piece bitboard copy
        while(bitboard) {
            sourceTile = BitBoard::getLSHBIndex(bitboard);
            //init piece attacks in order to get set of target squares, make sure not to attack same colored pieces
            attacks = BitBoard::getRookAttacks(sourceTile, BitBoard::bbState.occupancies[Both]) & ((BitBoard::bbState.side == White) ? ~BitBoard::bbState.occupancies[White] : ~BitBoard::bbState.occupancies[Black]);
            while(attacks) { //loop over tiles in attacks
                targetTile = BitBoard::getLSHBIndex(attacks);
                //quiet moves
                if(!getBit(((BitBoard::bbState.side == White) ? BitBoard::bbState.occupancies[Black] : BitBoard::bbState.occupancies[White]), targetTile)) {
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 0, 0, 0, 0));
                } else {
                    //capture moves
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 1, 0, 0, 0));
                }
                popBit(attacks, targetTile);
            }
            //pop LSHB
            popBit(bitboard, sourceTile);
        }
    }
}

void Movement::bitQueenMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if((BitBoard::bbState.side == White) ? piece == Q : piece == q) {
        //loop over source tiles of piece bitboard copy
        while(bitboard) {
            sourceTile = BitBoard::getLSHBIndex(bitboard);
            //init piece attacks in order to get set of target squares, make sure not to attack same colored pieces
            attacks = BitBoard::getQueenAttacks(sourceTile, BitBoard::bbState.occupancies[Both]) & ((BitBoard::bbState.side == White) ? ~BitBoard::bbState.occupancies[White] : ~BitBoard::bbState.occupancies[Black]);
            while(attacks) { //loop over tiles in attacks
                targetTile = BitBoard::getLSHBIndex(attacks);
                //quiet moves
                if(!getBit(((BitBoard::bbState.side == White) ? BitBoard::bbState.occupancies[Black] : BitBoard::bbState.occupancies[White]), targetTile)) {
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 0, 0, 0, 0));
                } else {
                    //capture moves
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 1, 0, 0, 0));
                }
                popBit(attacks, targetTile);
            }
            //pop LSHB
            popBit(bitboard, sourceTile);
        }
    }
}

void Movement::bitKingMoves(Moves &moveList, int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if((BitBoard::bbState.side == White) ? piece == K : piece == k) {
        //loop over source tiles of piece bitboard copy
        while(bitboard) {
            sourceTile = BitBoard::getLSHBIndex(bitboard);
            //init piece attacks in order to get set of target squares, make sure not to attack same colored pieces
            attacks = BitBoard::bbState.kingAttack[sourceTile] & ((BitBoard::bbState.side == White) ? ~BitBoard::bbState.occupancies[White] : ~BitBoard::bbState.occupancies[Black]);
            while(attacks) { //loop over tiles in attacks
                targetTile = BitBoard::getLSHBIndex(attacks);
                //quiet moves
                if(!getBit(((BitBoard::bbState.side == White) ? BitBoard::bbState.occupancies[Black] : BitBoard::bbState.occupancies[White]), targetTile)) {
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 0, 0, 0, 0));
                } else {
                    //capture moves
                    addMove(moveList, encodeMove(sourceTile, targetTile, piece, 0, 1, 0, 0, 0));
                }
                popBit(attacks, targetTile);
            }
            //pop LSHB
            popBit(bitboard, sourceTile);
        }
    }
}

int Movement::makeMove(int move, int moveFlag) {
    if(moveFlag == allMoves) {
        //quiet moves
        //preserve board state
        BitBoard::copyBitBoard();
        //parse move
        int sourceTile = getMoveSource(move);
        int targetTile = getMoveTarget(move);
        int piece = getMovePiece(move);
        int promoPiece = getMovePromo(move);
        int capture = getMoveCapture(move);
        int doublePPush = getDoublePPush(move);
        int enpass = getEnPassant(move);
        int castling = getCastling(move);

        //move piece 
        popBit(BitBoard::bbState.pieceBitboards[piece], sourceTile); //take bit off initial tile
        setBitOn(BitBoard::bbState.pieceBitboards[piece], targetTile); //put it on the new tile

        //capture moves
        if(capture) {
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
                if(getBit(BitBoard::bbState.pieceBitboards[bbPiece], targetTile)) {
                    //if piece on target tile we remove from that bitboard
                    popBit(BitBoard::bbState.pieceBitboards[bbPiece], targetTile);
                    break;
                }
            }
        }
        //pawn promotions
        if(promoPiece) {
            popBit(BitBoard::bbState.pieceBitboards[(BitBoard::bbState.side == White ? P : p)], targetTile);
            setBitOn(BitBoard::bbState.pieceBitboards[promoPiece], targetTile);
        }

        //en passant capture 
        if(enpass) {
            (BitBoard::bbState.side == White) ? popBit(BitBoard::bbState.pieceBitboards[p], (targetTile + 8))
                                                : popBit(BitBoard::bbState.pieceBitboards[P], (targetTile - 8));
        }
        BitBoard::bbState.enpassant = noTile;

        //double pawn push, setting up en passant tile
        if(doublePPush) {
            (BitBoard::bbState.side == White ? (BitBoard::bbState.enpassant = targetTile + 8) 
                                            : (BitBoard::bbState.enpassant = targetTile - 8));
        }

        //castling moves
        if(castling) {
            switch (targetTile) {
                //WKCA
                case (g1):
                    popBit(BitBoard::bbState.pieceBitboards[R], h1);
                    setBitOn(BitBoard::bbState.pieceBitboards[R], f1);
                    break;
                case (c1):
                    popBit(BitBoard::bbState.pieceBitboards[R], a1);
                    setBitOn(BitBoard::bbState.pieceBitboards[R], d1);
                    break;
                case (g8):
                    popBit(BitBoard::bbState.pieceBitboards[r], h8);
                    setBitOn(BitBoard::bbState.pieceBitboards[r], f8);
                    break;
                case (c8):
                    popBit(BitBoard::bbState.pieceBitboards[r], a8);
                    setBitOn(BitBoard::bbState.pieceBitboards[r], d8);
                    break;
                
            }
        }
        //update castling rights
        BitBoard::bbState.canCastle &= castlingRights[sourceTile];
        BitBoard::bbState.canCastle &= castlingRights[targetTile];

        //update occupancies
        BitBoard::bbState.occupancies[White] = ZERO;
        BitBoard::bbState.occupancies[Black] = ZERO;
        BitBoard::bbState.occupancies[Both] = ZERO;
        for(int bbPiece = P; bbPiece <= K; ++bbPiece) {
            BitBoard::bbState.occupancies[White] |= BitBoard::bbState.pieceBitboards[bbPiece];
        }
        for(int bbPiece = p; bbPiece <= k; ++bbPiece) {
            BitBoard::bbState.occupancies[Black] |= BitBoard::bbState.pieceBitboards[bbPiece];
        }
        BitBoard::bbState.occupancies[Both] |= BitBoard::bbState.occupancies[White];
        BitBoard::bbState.occupancies[Both] |= BitBoard::bbState.occupancies[Black];
        
        //change side
        BitBoard::bbState.side ^= 1;

        //make sure king has not been exposed into a check
        if(BitBoard::isTileAttacked((BitBoard::bbState.side == White) 
            ? BitBoard::getLSHBIndex(BitBoard::bbState.pieceBitboards[k]) 
                : BitBoard::getLSHBIndex(BitBoard::bbState.pieceBitboards[K]), BitBoard::bbState.side)) {
            BitBoard::restoreBitBoard();
            return 0;
        } else {
            return 1;
        }
    } else {
        //move is capture   
        if(getMoveCapture(move)) {
            makeMove(move, allMoves);
        } else {
            //move is not a capture, dont make it
            return 0;
        }     
    }
    return 0;
}

