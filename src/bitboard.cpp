#include "bitboard.hpp"

void BitBoard::printBitBoard(uint64_t bitBoardIn) {
    uint64_t one = 1;
    for(std::size_t rank = 0; rank < 8; ++rank) {
        for(std::size_t file = 0; file < 8; ++file) {
            std::size_t tile = rank * 8 + file;
            if(!file) {
                std::cout << 8 - rank << "  ";
            }
            uint64_t result = (bitBoardIn & (one << tile)) ? 1 : 0; //if(bitBoardIn & (one >> tile) == 1), result = 1; else result = 0
            std::cout << result << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n";
    std::cout << "--------------------\n\n";
    
}

uint64_t BitBoard::setOccupancy(int index, int bitsInMask, uint64_t attackMaskIn) {
    uint64_t occupancy = ZERO;
    //loop over the bits in attack mask
    for(int count = 0; count < bitsInMask; ++count) {
        //get LSHB index of attackMask
        int tile = getLSHBIndex(attackMaskIn);
        //pop LSHB in attackMask
        popBit(attackMaskIn, tile);

        if(index & (1 << count)) {
            occupancy |= (ONE << tile);
        }
    }

    return occupancy;
}

uint64_t BitBoard::maskPawnAttacks(int tile, Color colorIn) {
    uint64_t attacks = ZERO;
    uint64_t bitboardIn = ZERO;

    setBitOn(bitboardIn, tile);
    std::cout << "\n";
    if(colorIn == White) {
        if((bitboardIn >> 7) & notAfile) {
            attacks |= (bitboardIn >> 7); //attack up left
        }
            
        if((bitboardIn >> 9) & notHfile) {
            attacks |= (bitboardIn >> 9); //attack up right
        }

    } else { //Black
        if((bitboardIn << 7) & notHfile) {
            attacks |= (bitboardIn << 7);  //attack down right (up left from blacks pov)
        }
        if((bitboardIn << 9) & notAfile) {
            attacks |= (bitboardIn << 9); //attack down left (up right from blacks pov)
        }
    }
    return attacks;
}

uint64_t BitBoard::maskKnightAttacks(int tile) {
    uint64_t attacks = ZERO;
    uint64_t bitboardIn = ZERO;

    setBitOn(bitboardIn, tile);

    if((bitboardIn >> 17) & notHfile) {
        attacks |= (bitboardIn >> 17); //up 2 right 1
    }
    if((bitboardIn >> 15) & notAfile) {
        attacks |= (bitboardIn >> 15); //up 2 left 1
    }
    if((bitboardIn >> 10) & notHGfile) {
        attacks |= (bitboardIn >> 10); //up 1 right 2
    }
    if((bitboardIn >> 6) & notABfile) {
        attacks |= (bitboardIn >> 6); //up 1 left 2
    }
    if((bitboardIn << 17) & notAfile) {
        attacks |= (bitboardIn << 17); //up 2 right 1
    }
    if((bitboardIn << 15) & notHfile) {
        attacks |= (bitboardIn << 15); //up 2 left 1
    }
    if((bitboardIn << 10) & notABfile) {
        attacks |= (bitboardIn << 10); //up 1 right 2
    }
    if((bitboardIn << 6) & notHGfile) {
        attacks |= (bitboardIn << 6); //up 1 left 2
    }
    

    return attacks;
}

uint64_t BitBoard::maskKingAttacks(int tile) {
    uint64_t attacks = ZERO;
    uint64_t bitboardIn = ZERO;

    setBitOn(bitboardIn, tile);

    if(bitboardIn >> 8) {
        attacks |= (bitboardIn >> 8); //up 1
    }
    if((bitboardIn >> 9) & notHfile) {
        attacks |= (bitboardIn >> 9); //up right 1
    }
    if((bitboardIn >> 7) & notAfile) {
        attacks |= (bitboardIn >> 7); //up left 1
    }
    if((bitboardIn >> 1) & notHfile) {
        attacks |= (bitboardIn >> 1); //right 1
    }
    if((bitboardIn << 1) & notAfile) {
        attacks |= (bitboardIn << 1); //left 1
    }
    if((bitboardIn << 9) & notAfile) {
        attacks |= (bitboardIn << 9); //down right 1
    }
    if((bitboardIn << 7) & notHfile) {
        attacks |= (bitboardIn << 7); //down left 1
    }
    if(bitboardIn << 8) {
        attacks |= (bitboardIn << 8); //down 1
    }

    return attacks;
}

void BitBoard::initLeaperAttacks() {//magic numbers for bishop
    for(int tile = 0; tile < 64; ++tile) {
        pawnAttack[White][tile] = maskPawnAttacks(tile, White);
        pawnAttack[Black][tile] = maskPawnAttacks(tile, Black);
        knightAttack[tile] = maskKnightAttacks(tile);
        kingAttack[tile] = maskKingAttacks(tile);
    }
}

uint64_t BitBoard::maskBishopAttacks(int tile) {
    uint64_t attacks = ZERO;
    int rank;
    int file;
    
    int targetR = tile / 8;
    int targetF = tile % 8;
    for(rank = targetR + 1, file = targetF + 1; rank <= 6 && file <= 6; ++rank, ++file) { //down right
        attacks |= (ONE << (rank * 8 + file)); 
    }
    for(rank = targetR - 1, file = targetF + 1; rank >= 1 && file <= 6; --rank, ++file) { //up right
        attacks |= (ONE << (rank * 8 + file)); 
    }
    for(rank = targetR + 1, file = targetF - 1; rank <= 6 && file >= 1; ++rank, --file) { //down left
        attacks |= (ONE << (rank * 8 + file)); 
    }
    for(rank = targetR - 1, file = targetF - 1; rank >= 1 && file >= 1; --rank, --file) { //up left
        attacks |= (ONE << (rank * 8 + file)); 
    }
    return attacks;
}

uint64_t BitBoard::BishopOnFly(int tile, uint64_t block) {
    uint64_t attacks = ZERO;
    int rank;
    int file;
    
    int targetR = tile / 8;
    int targetF = tile % 8;

    //generate bishop attacks
    for(rank = targetR + 1, file = targetF + 1; rank <= 7 && file <= 7; ++rank, ++file) { //down right
        attacks |= (ONE << (rank * 8 + file)); 
        if((ONE << (rank * 8 + file)) & block) { //taking care of blockers ignoring color
            break;
        }
    }
    for(rank = targetR - 1, file = targetF + 1; rank >= 0 && file <= 7; --rank, ++file) { //up right
        attacks |= (ONE << (rank * 8 + file)); 
        if((ONE << (rank * 8 + file)) & block) { //taking care of blockers ignoring color
            break;
        }
    }
    for(rank = targetR + 1, file = targetF - 1; rank <= 7 && file >= 0; ++rank, --file) { //down left
        attacks |= (ONE << (rank * 8 + file)); 
        if((ONE << (rank * 8 + file)) & block) { //taking care of blockers ignoring color
            break;
        }
    }
    for(rank = targetR - 1, file = targetF - 1; rank >= 0 && file >= 0; --rank, --file) { //up left
        attacks |= (ONE << (rank * 8 + file)); 
        if((ONE << (rank * 8 + file)) & block) { //taking care of blockers ignoring color
            break;
        }
    }

    return attacks;
}

uint64_t BitBoard::maskRookAttacks(int tile) {
    uint64_t attacks = ZERO;
    int rank;
    int file;
    
    int targetR = tile / 8;
    int targetF = tile % 8;
    for(rank = targetR + 1; rank <= 6; ++rank) { //down
        attacks |= (ONE << (rank * 8 + targetF)); 
    }
    for(rank = targetR - 1; rank >= 1; --rank) { //up
        attacks |= (ONE << (rank * 8 + targetF)); 
    }
    for(file = targetF + 1; file <= 6; ++file) { //right
        attacks |= (ONE << (targetR * 8 + file)); 
    }
    for(file = targetF - 1; file >= 1; --file) { //left
        attacks |= (ONE << (targetR * 8 + file)); 
    }

    return attacks;
}

uint64_t BitBoard::RookOnFly(int tile, uint64_t block) {
    uint64_t attacks = ZERO;
    int rank;
    int file;
    
    int targetR = tile / 8;
    int targetF = tile % 8;

    //generate rook attacks
    for(rank = targetR + 1; rank <= 7; ++rank) { //down
        attacks |= (ONE << (rank * 8 + targetF)); 
        if((ONE << (rank * 8 + targetF)) & block) {
            break;
        }    
    }
    for(rank = targetR - 1; rank >= 0; --rank) { //up
        attacks |= (ONE << (rank * 8 + targetF)); 
        if((ONE << (rank * 8 + targetF)) & block) {
            break;
        }
    }
    for(file = targetF + 1; file <= 7; ++file) { //right
        attacks |= (ONE << (targetR * 8 + file));
        if((ONE << (targetR * 8 + file)) & block) {
            break;
        }
    }
    for(file = targetF - 1; file >= 0; --file) { //left
        attacks |= (ONE << (targetR * 8 + file)); 
        if((ONE << (targetR * 8 + file)) & block) {
            break;
        }
    }

    return attacks;
}

void BitBoard::initSliderAttacks(RB pieceIn) {
    for(int tile = 0; tile < 64; ++tile) {
        bishopMasks[tile] = maskBishopAttacks(tile);
        rookMasks[tile] = maskRookAttacks(tile);
        uint64_t attackMask;
        if(pieceIn == bishop) {
            attackMask = bishopMasks[tile];
        } else {
            attackMask = rookMasks[tile];
        }
        

        int relevantBits = countBitsOn(attackMask);

        int occupancyIndicies = (1 << relevantBits);
        for(int i = 0; i < occupancyIndicies; ++i) {
            if(pieceIn == bishop) {
                //init current occupancy variation
                uint64_t occupancy = setOccupancy(i, relevantBits, attackMask);
                //init magic index
                int magicIndex = (occupancy * m.bishopMagicNumbers[tile]) >> (64 - bishopRelevantBits[tile]);

                //init bishop attacks
                bishopAttacks[tile][magicIndex] = BishopOnFly(tile, occupancy);
            } else {//rook
                //init current occupancy variation
                uint64_t occupancy = setOccupancy(i, relevantBits, attackMask);
                //init magic index
                int magicIndex = (occupancy * m.rookMagicNumbers[tile]) >> (64 - rookRelevantBits[tile]);

                //init rook attacks
                rookAttacks[tile][magicIndex] = RookOnFly(tile, occupancy);
            }
        }
    }
}

void BitBoard::printBoard() {
    for(int rank = 0; rank < 8; ++rank) {
        for(int file = 0; file < 8; ++file) {
            int tile = rank * 8 + file;
            if(!file) {
                std::cout  << 8 - rank << " ";
            }

            int piece = -1;

            //loop over all piece bitboards
            for(int bbPiece = P; bbPiece <= k; ++bbPiece) {
                //if there is a piece on current tile the we get the piece code
                if(getBit(pieceBitboards[bbPiece], tile)) {
                    piece = bbPiece;
                }
            }
            std::string out;
            if(piece == -1) {
                out = ".";
            } else {
                out = unicodePieces[piece];
            }
            std::cout << " " << out;
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n";
    std::cout << "--------------------\n\n";

    if(side == White) {
        std::cout << "side to move: " << "white\n";
    } else {
        std::cout << "side to move: " << "black\n";
    }

    if(enpassant != noTile) {
        std::cout << "En passant: " << tileToCoord[enpassant] << "\n";
    } else {
        std::cout << "En passant: " <<  "no\n\n";
    }

    std::cout << "Castling: ";
    if(canCastle & WKCA) {
        std::cout<< "wK";
    } else {
        std::cout << '-';
    }
    if(canCastle & WQCA) {
        std::cout << "wQ";
    } else {
        std::cout <<  "-";
    }
    if(canCastle & BKCA) {
        std::cout << "bK";
    } else {
        std::cout << "-";
    }
    if(canCastle & BQCA) {
        std::cout << "bQ";
    } else {
        std::cout << '-';
    }
    std::cout << "\n\n";
    
    std::cout << "--------------------\n\n";
    

}

void BitBoard::parseFEN(std::string fen) {
    memset(pieceBitboards, ZERO, sizeof(pieceBitboards));
    memset(occupancies, ZERO, sizeof(occupancies));
    side = White;
    enpassant = noTile;
    canCastle = 0;
    int count = 0;
    for(int rank = 0; rank < 8; ++rank) {
        for(int file = 0; file < 8; ++file) {
            //init current tile
            int tile = rank * 8 + file;
            if(((fen[count] >= 'a') && (fen[count] <= 'z')) || ((fen[count] >= 'A') && (fen[count] <= 'Z'))) {
                //init piece type
                int out = fen[count];
                int piece = charPieces[out];
                setBitOn(pieceBitboards[piece], tile);
                ++count;
            } 
            //empty tiles
            if(fen[count] >= '0' && fen[count] <= '9') {
                int offset = fen[count] - '0';
                int piece = -1;
                for(int bbPiece = P; bbPiece <= k; ++bbPiece) {
                    //if there is a piece on current tile the we get the piece code
                    if(getBit(pieceBitboards[bbPiece], tile)) {
                        piece = bbPiece;
                    }
                }
                //if no piece on current tile
                if(piece == -1) {
                    //decrement file to deal with right shift offset
                    --file;
                }
                file += offset;
                ++count;
            }     
            //match rank seperator
            if(fen[count] == '/') {
                count++;
            }  
        }
    }
    //once we finish pieces we need to parse the state variables
    ++count;
    //parse side to move
    if(fen[count] == 'w') {
        side = White;
    } else {
        side = Black;
    }
    count += 2;
    //parse castling
    while(fen[count] != ' ') {
        switch(fen[count]) {
            case 'K':
                canCastle |= WKCA;
                break;
            case 'Q':
                canCastle |= WQCA;
                break;
            case 'k':
                canCastle |= BKCA;
                break;
            case 'q':
                canCastle |= BQCA;
                break;
            case '-':
                break;
        }
        ++count;
    }
    ++count;
    //parse enpassant
    if(fen[count] != '-') {
        //parse enpassant file and rank
        int file = fen[count] - 'a';
        int rank = 8 - (fen[count + 1] - '0');
        enpassant = rank * 8 + file;
    } else { //no enpassant
        enpassant = noTile;
    }
    //loop over white pieces bitboards
    for(int piece = P; piece <= K; ++piece) {
        occupancies[White] |= pieceBitboards[piece];
    }
    for(int piece = p; piece <= k; ++piece) {
        occupancies[Black] |= pieceBitboards[piece];
    }
    occupancies[Both] |= (occupancies[White] | occupancies[Black]);
    std::cout << "fen: " << fen << "\n";
}

bool BitBoard::isTileAttacked(int tile, int side) {
    //if attacked by white pawns
    if((side == White) && (pawnAttack[Black][tile] & pieceBitboards[P])) {
        return true;
    }
    //if attack by black pawn
    if((side == Black) && (pawnAttack[White][tile] & pieceBitboards[p])) {
        return true;
    }
    //attacked by knight
    if(knightAttack[tile] & ((side == White) ? pieceBitboards[N] : pieceBitboards[n])) {
        return true;
    }
    //attacked by bishop
    if(getBishopAttacks(tile, occupancies[Both]) & ((side == White) ? pieceBitboards[B] : pieceBitboards[b])) {
        return true;
    }
    //attacked by rook
    if(getRookAttacks(tile, occupancies[Both]) & ((side == White) ? pieceBitboards[R] : pieceBitboards[r])) {
        return true;
    }
    //attacked by queen
    if(getQueenAttacks(tile, occupancies[Both]) & ((side == White) ? pieceBitboards[Q] : pieceBitboards[q])) {
        return true;
    }
    //attacked by king
    if(kingAttack[tile] & ((side == White) ? pieceBitboards[K] : pieceBitboards[k])) {
        return true;
    }
    return false;
}

void BitBoard::printAttackedSquares(int side) {
    for(int rank = 0; rank < 8; ++rank) {
        for(int file = 0; file < 8; ++file) {
            int tile = rank * 8 + file;
            if(!file) {
                std::cout << "   " << 8 - rank;
            }
            std::cout << " " << isTileAttacked(tile, side);
        }
        std::cout << "\n";
    }
    std::cout << "     a b c d e f g h\n\n";
}

void BitBoard::generateMoves() {
    int sourceTile = 0;
    int targetTile = 0;
    //define current piece bitboard copy and attacks
    uint64_t bitboard = ZERO;
    uint64_t attacks = ZERO;
    for(int piece = P; piece <= k; ++piece) {
        bitboard = pieceBitboards[piece];
        //white pawn moves, white king castle moves
        if(side == White) {
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

void BitBoard::bitPawnMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if(piece == P && side == White) {
        //loop over white pawns in white pawn bitboard
        while(bitboard) {
            sourceTile = getLSHBIndex(bitboard);
            targetTile = sourceTile - 8;
            //generate quiet pawn moves
            if(!(targetTile < a8) && !getBit(occupancies[Both], targetTile)) {
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
                    if((sourceTile >= a2 && sourceTile <= h2) && !getBit(occupancies[Both], (targetTile - 8))) {
                        std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile - 8] << "   double pawn push\n";
                    }
                }
            }
            //init white pawn attacks bitboard
            attacks = pawnAttack[side][sourceTile] & occupancies[Black];
            while(attacks) {
                targetTile = getLSHBIndex(attacks);
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
            if(enpassant != noTile) {
                uint64_t enpassantAttacks = pawnAttack[side][sourceTile] & (ONE << enpassant);
                //check if enpassant capture is available
                if(enpassantAttacks) {
                    int enpassantTargetTile = getLSHBIndex(enpassantAttacks);
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[enpassantTargetTile] << "   pawn en passant capture\n" ;
                }
            }
            //pop LSHB from bitboard
            popBit(bitboard, sourceTile);
        }
    } else if(piece == p && side == Black) {
        //loop over black pawns in black pawn bitboard
        while(bitboard) {
            sourceTile = getLSHBIndex(bitboard);
            targetTile = sourceTile + 8;
            //generate quiet pawn moves
            if(!(targetTile > h1) && !getBit(occupancies[Both], targetTile)) {
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
                    if((sourceTile >= a7 && sourceTile <= h7) && !getBit(occupancies[Both], (targetTile + 8))) {
                        std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[targetTile + 8] << "   double pawn push\n";
                    }
                }
            }
            //init black pawn attacks bitboard
            attacks = pawnAttack[side][sourceTile] & occupancies[White];
            while(attacks) {
                targetTile = getLSHBIndex(attacks);
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
            if(enpassant != noTile) {
                uint64_t enpassantAttacks = pawnAttack[side][sourceTile] & (ONE << enpassant);
                //check if enpassant capture is available
                if(enpassantAttacks) {
                    int enpassantTargetTile = getLSHBIndex(enpassantAttacks);
                    std::cout << tileToCoord[sourceTile] << "->" << tileToCoord[enpassantTargetTile] << "   pawn en passant capture\n";
                }
            }
            //pop LSHB from bitboard
            popBit(bitboard, sourceTile);
        }
    }
}

void BitBoard::bitCastling(int piece) {
    if(piece == K && side == White) {
        //king side castling
        if(canCastle & WKCA) {
            //make sure squares between king and king side rook are free
            if(!getBit(occupancies[Both], f1) && !getBit(occupancies[Both], g1)) {
                //make sure king and the f1 tile are not targeted by enemy pieces 
                if(!isTileAttacked(e1, Black) && !isTileAttacked(f1, Black)) {
                    std::cout << "e1->g1" << "   castling move\n";
                }
            }
        } //canCastle 
        //queen side castling
        if(canCastle & WQCA) {
            //make sure squares between king and queen side rook are free
            if(!getBit(occupancies[Both], d1) && !getBit(occupancies[Both], c1) && !getBit(occupancies[Both], b1)) {
                //make sure king and the d1 tile are not targeted by enemy pieces 
                if(!isTileAttacked(e1, Black) && !isTileAttacked(d1, Black)) {
                    std::cout << "e1->c1" << "   castling move\n";
                }
            }
        }
    } else {
        if(piece == k && side == Black) {
            //king side castling
            if(canCastle & BKCA) {
                //make sure squares between king and king side rook are free
                if(!getBit(occupancies[Both], f8) && !getBit(occupancies[Both], g8)) {
                    //make sure king and the f8 tile are not targeted by enemy pieces 
                    if(!isTileAttacked(e8, White) && !isTileAttacked(f8, White)) {
                        std::cout << "e8->g8" << "   castling move\n";
                    }
                }
            }
            //queen side castling
            if(canCastle & BQCA) {
                //make sure squares between king and queen side rook are free
                if(!getBit(occupancies[Both], d8) && !getBit(occupancies[Both], c8) && !getBit(occupancies[Both], b8)) {
                    //make sure king and the d8 tile are not targeted by enemy pieces 
                    if(!isTileAttacked(e8, White) && !isTileAttacked(d8, White)) {
                        std::cout << "e8->c8" << "   castling move\n";
                    }
                }
            }
        }
    }
}

void BitBoard::bitKnightMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if((side == White) ? piece == N : piece == n) {
        //loop over source tiles of piece bitboard copy
        while(bitboard) {
            sourceTile = getLSHBIndex(bitboard);
            //init piece attacks in order to get set of target squares, make sure not to attack same colored pieces
            attacks = knightAttack[sourceTile] & ((side == White) ? ~occupancies[White] : ~occupancies[Black]);
            while(attacks) { //loop over tiles in attacks
                targetTile = getLSHBIndex(attacks);
                //quiet moves
                if(!getBit(((side == White) ? occupancies[Black] : occupancies[White]), targetTile)) {
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

void BitBoard::bitBishopMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if((side == White) ? piece == B : piece == b) {
        //loop over source tiles of piece bitboard copy
        while(bitboard) {
            sourceTile = getLSHBIndex(bitboard);
            //init piece attacks in order to get set of target squares, make sure not to attack same colored pieces
            attacks = getBishopAttacks(sourceTile, occupancies[Both]) & ((side == White) ? ~occupancies[White] : ~occupancies[Black]);
            while(attacks) { //loop over tiles in attacks
                targetTile = getLSHBIndex(attacks);
                //quiet moves
                if(!getBit(((side == White) ? occupancies[Black] : occupancies[White]), targetTile)) {
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

void BitBoard::bitRookMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if((side == White) ? piece == R : piece == r) {
        //loop over source tiles of piece bitboard copy
        while(bitboard) {
            sourceTile = getLSHBIndex(bitboard);
            //init piece attacks in order to get set of target squares, make sure not to attack same colored pieces
            attacks = getRookAttacks(sourceTile, occupancies[Both]) & ((side == White) ? ~occupancies[White] : ~occupancies[Black]);
            while(attacks) { //loop over tiles in attacks
                targetTile = getLSHBIndex(attacks);
                //quiet moves
                if(!getBit(((side == White) ? occupancies[Black] : occupancies[White]), targetTile)) {
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

void BitBoard::bitQueenMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if((side == White) ? piece == Q : piece == q) {
        //loop over source tiles of piece bitboard copy
        while(bitboard) {
            sourceTile = getLSHBIndex(bitboard);
            //init piece attacks in order to get set of target squares, make sure not to attack same colored pieces
            attacks = getQueenAttacks(sourceTile, occupancies[Both]) & ((side == White) ? ~occupancies[White] : ~occupancies[Black]);
            while(attacks) { //loop over tiles in attacks
                targetTile = getLSHBIndex(attacks);
                //quiet moves
                if(!getBit(((side == White) ? occupancies[Black] : occupancies[White]), targetTile)) {
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

void BitBoard::bitKingMoves(int piece, uint64_t bitboard, int sourceTile, int targetTile, uint64_t attacks) {
    if((side == White) ? piece == K : piece == k) {
        //loop over source tiles of piece bitboard copy
        while(bitboard) {
            sourceTile = getLSHBIndex(bitboard);
            //init piece attacks in order to get set of target squares, make sure not to attack same colored pieces
            attacks = kingAttack[sourceTile] & ((side == White) ? ~occupancies[White] : ~occupancies[Black]);
            while(attacks) { //loop over tiles in attacks
                targetTile = getLSHBIndex(attacks);
                //quiet moves
                if(!getBit(((side == White) ? occupancies[Black] : occupancies[White]), targetTile)) {
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
