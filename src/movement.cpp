#include "movement.hpp"
#include "bitboard.hpp"


const char promotedPieces[] = {
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n',
};

void Movement::printMove(int move) {
    std::cout << tileToCoord[getMoveSource(move)] 
            << tileToCoord[getMoveTarget(move)]
            << promotedPieces[getMovePromo(move)] << "\n"; 
}

void Movement::printMoveList() { 
    std::cout << "\n    move   piece  capture   double   enpassant   castling\n\n";
    for(int count = 0; count < moveStruct.moveCount; ++count) {
        int move = moveStruct.moves[count];

        //print move
        std::cout << "    " << tileToCoord[getMoveSource(move)] 
            << tileToCoord[getMoveTarget(move)]
            << promotedPieces[getMovePromo(move)] << "  "
            << unicodePieces[getMovePiece(move)] << "      "
            << (getMoveCapture(move) ? 1 : 0) << "         " 
            << (getDoublePPush(move) ? 1 : 0) << "        "
            << (getEnPassant(move) ? 1 : 0) << "           "
            << (getCastling(move) ? 1 : 0) << "\n";  
        
            std::cout << "\n    Total number of moves: " << moveStruct.moveCount << "\n";
        
    }    
}

void Movement::generateMoves() {
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
            bitPawnMoves(piece, bitboard, sourceTile, targetTile, attacks);
            //white castling moves
            bitCastling(piece);
        } else { //black pawn moves, black king castle moves
            //black pawn moves
            bitPawnMoves(piece, bitboard, sourceTile, targetTile, attacks);
            //black castling moves
            bitCastling(piece);
        }
        //knight moves
        bitKnightMoves(piece, bitboard, sourceTile, targetTile, attacks);
        //bishop moves
        bitBishopMoves(piece, bitboard, sourceTile, targetTile, attacks);
        //rook moves
        bitRookMoves(piece, bitboard, sourceTile, targetTile, attacks);
        //queen moves
        bitQueenMoves(piece, bitboard, sourceTile, targetTile, attacks);
        //king moves (non castle)
        bitKingMoves(piece, bitboard, sourceTile, targetTile, attacks);
    }
}

void Movement::bitPawnMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if(piece == P && BitBoard::bbState.side == White) {
        //loop over white pawns in white pawn bitboard
        while(bitboard) {
            sourceTile = BitBoard::getLSHBIndex(bitboard);
            targetTile = sourceTile - 8;
            //generate quiet pawn moves
            if(!(targetTile < a8) && !getBit(BitBoard::bbState.occupancies[Both], targetTile)) {
                //pawn promo
                if(sourceTile >= a7 && sourceTile <= h7) {
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn promotion: Q\n" ;
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn promotion: B\n" ;
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn promotion: B\n" ;
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn promotion: N\n" ;
                } else { //pawn pushes
                    // one square pawn push
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn push\n";
                    //two square pawn push
                    if((sourceTile >= a2 && sourceTile <= h2) && !getBit(BitBoard::bbState.occupancies[Both], (targetTile - 8))) {
                        std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile - 8] << "   double pawn push\n";
                    }
                }
            }
            //init white pawn attacks bitboard
            attacks = BitBoard::bbState.pawnAttack[BitBoard::bbState.side][sourceTile] & BitBoard::bbState.occupancies[Black];
            while(attacks) {
                targetTile = BitBoard::getLSHBIndex(attacks);
                if(sourceTile >= a7 && sourceTile <= h7) {
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn capture promotion: Q\n";
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn capture promotion: R\n";
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn capture promotion: B\n";
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn capture promotion: N\n";
                } else { //pawn pushes
                    // one square pawn push
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn capture\n";
                }
                popBit(attacks, targetTile);
            }
            //generate enpassant captures
            if(BitBoard::bbState.enpassant != noTile) {
                uint64_t enpassantAttacks = BitBoard::bbState.pawnAttack[BitBoard::bbState.side][sourceTile] & (ONE << BitBoard::bbState.enpassant);
                //check if enpassant capture is available
                if(enpassantAttacks) {
                    int enpassantTargetTile = BitBoard::getLSHBIndex(enpassantAttacks);
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[enpassantTargetTile] << "   pawn en passant capture\n" ;
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
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn promotion: q\n";
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn promotion: r\n";
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn promotion: b\n";
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn promotion: n\n";
                } else { //pawn pushes
                    // one square pawn push
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn push:\n";
                    //two square pawn push
                    if((sourceTile >= a7 && sourceTile <= h7) && !getBit(BitBoard::bbState.occupancies[Both], (targetTile + 8))) {
                        std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile + 8] << "   double pawn push\n";
                    }
                }
            }
            //init black pawn attacks bitboard
            attacks = BitBoard::bbState.pawnAttack[BitBoard::bbState.side][sourceTile] & BitBoard::bbState.occupancies[White];
            while(attacks) {
                targetTile = BitBoard::getLSHBIndex(attacks);
                if(sourceTile >= a2 && sourceTile <= h2) {
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn capture promotion: q\n";
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn capture promotion: r\n";
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn capture promotion: b\n";
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn capture promotion: n\n";
                } else { //pawn pushes
                    // one square pawn push
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   pawn capture\n";
                }
                popBit(attacks, targetTile);
            }
            //generate enpassant captures
            if(BitBoard::bbState.enpassant != noTile) {
                uint64_t enpassantAttacks = BitBoard::bbState.pawnAttack[BitBoard::bbState.side][sourceTile] & (ONE << BitBoard::bbState.enpassant);
                //check if enpassant capture is available
                if(enpassantAttacks) {
                    int enpassantTargetTile = BitBoard::getLSHBIndex(enpassantAttacks);
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[enpassantTargetTile] << "   pawn en passant capture\n";
                }
            }
            //pop LSHB from bitboard
            popBit(bitboard, sourceTile);
        }
    }
}

void Movement::bitCastling(int piece) {
    if(piece == K && BitBoard::bbState.side == White) {
        //king side castling
        if(BitBoard::bbState.canCastle & WKCA) {
            //make sure squares between king and king side rook are free
            if(!getBit(BitBoard::bbState.occupancies[Both], f1) && !getBit(BitBoard::bbState.occupancies[Both], g1)) {
                //make sure king and the f1 tile are not targeted by enemy pieces 
                if(!BitBoard::isTileAttacked(e1, Black) && !BitBoard::isTileAttacked(f1, Black)) {
                    std::cout << "e1->g1" << "   castling move\n";
                }
            }
        } //canCastle 
        //queen side castling
        if(BitBoard::bbState.canCastle & WQCA) {
            //make sure squares between king and queen side rook are free
            if(!getBit(BitBoard::bbState.occupancies[Both], d1) && !getBit(BitBoard::bbState.occupancies[Both], c1) && !getBit(BitBoard::bbState.occupancies[Both], b1)) {
                //make sure king and the d1 tile are not targeted by enemy pieces 
                if(!BitBoard::isTileAttacked(e1, Black) && !BitBoard::isTileAttacked(d1, Black)) {
                    std::cout << "e1->c1" << "   castling move\n";
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
                        std::cout << "e8->g8" << "   castling move\n";
                    }
                }
            }
            //queen side castling
            if(BitBoard::bbState.canCastle & BQCA) {
                //make sure squares between king and queen side rook are free
                if(!getBit(BitBoard::bbState.occupancies[Both], d8) && !getBit(BitBoard::bbState.occupancies[Both], c8) && !getBit(BitBoard::bbState.occupancies[Both], b8)) {
                    //make sure king and the d8 tile are not targeted by enemy pieces 
                    if(!BitBoard::isTileAttacked(e8, White) && !BitBoard::isTileAttacked(d8, White)) {
                        std::cout << "e8->c8" << "   castling move\n";
                    }
                }
            }
        }
    }
}

void Movement::bitKnightMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
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
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   quiet move\n";
                } else {
                    //capture moves
                std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   piece capture\n";
                }
                popBit(attacks, targetTile);
            }
            //pop LSHB
            popBit(bitboard, sourceTile);
        }
    }
}

void Movement::bitBishopMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
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
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   quiet move\n";
                } else {
                    //capture moves
                std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   piece capture\n";
                }
                popBit(attacks, targetTile);
            }
            //pop LSHB
            popBit(bitboard, sourceTile);
        }
    }
}

void Movement::bitRookMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
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
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   quiet move\n";
                } else {
                    //capture moves
                std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   piece capture\n";
                }
                popBit(attacks, targetTile);
            }
            //pop LSHB
            popBit(bitboard, sourceTile);
        }
    }
}

void Movement::bitQueenMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
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
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   quiet move\n";
                } else {
                    //capture moves
                std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   piece capture\n";
                }
                popBit(attacks, targetTile);
            }
            //pop LSHB
            popBit(bitboard, sourceTile);
        }
    }
}

void Movement::bitKingMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
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
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   quiet move\n";
                } else {
                    //capture moves
                std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile] << "   piece capture\n";
                }
                popBit(attacks, targetTile);
            }
            //pop LSHB
            popBit(bitboard, sourceTile);
        }
    }
}

Moves Movement::moveStruct = Moves();  

