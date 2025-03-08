#include "bitboard.hpp"
#include <iostream>
BitBoard::BitBoardState BitBoard::bbState = BitBoard::BitBoardState();  

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

void BitBoard::initLeaperAttacks() {
    for(int tile = 0; tile < 64; ++tile) {
        bbState.pawnAttack[White][tile] = maskPawnAttacks(tile, White);
        bbState.pawnAttack[Black][tile] = maskPawnAttacks(tile, Black);
        bbState.knightAttack[tile] = maskKnightAttacks(tile);
        bbState.kingAttack[tile] = maskKingAttacks(tile);
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
        bbState.bishopMasks[tile] = maskBishopAttacks(tile);
        bbState.rookMasks[tile] = maskRookAttacks(tile);
        uint64_t attackMask;
        if(pieceIn == bishop) {
            attackMask = bbState.bishopMasks[tile];
        } else {
            attackMask = bbState.rookMasks[tile];
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
                bbState.bishopAttacks[tile][magicIndex] = BishopOnFly(tile, occupancy);
            } else {//rook
                //init current occupancy variation
                uint64_t occupancy = setOccupancy(i, relevantBits, attackMask);
                //init magic index
                int magicIndex = (occupancy * m.rookMagicNumbers[tile]) >> (64 - rookRelevantBits[tile]);

                //init rook attacks
                bbState.rookAttacks[tile][magicIndex] = RookOnFly(tile, occupancy);
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
                if(getBit(bbState.pieceBitboards[bbPiece], tile)) {
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

    if(bbState.side == White) {
        std::cout << "side to move: " << "white\n";
    } else {
        std::cout << "side to move: " << "black\n";
    }

    if(bbState.enpassant != noTile) {
        std::cout << "En passant: " << tileToCoord[bbState.enpassant] << "\n";
    } else {
        std::cout << "En passant: " <<  "no\n\n";
    }

    std::cout << "Castling: ";
    if(bbState.canCastle & WKCA) {
        std::cout<< "K";
    } else {
        std::cout << '-';
    }
    if(bbState.canCastle & WQCA) {
        std::cout << "Q";
    } else {
        std::cout <<  "-";
    }
    if(bbState.canCastle & BKCA) {
        std::cout << "k";
    } else {
        std::cout << "-";
    }
    if(bbState.canCastle & BQCA) {
        std::cout << "q";
    } else {
        std::cout << '-';
    }
    std::cout << "\n\n";
    
    std::cout << "--------------------\n\n";
    

}


bool BitBoard::isTileAttacked(int tile, int side) {
    //if attacked by white pawns
    if((side == White) && (bbState.pawnAttack[Black][tile] & bbState.pieceBitboards[P])) {
        return true;
    }
    //if attack by black pawn
    if((side == Black) && (bbState.pawnAttack[White][tile] & bbState.pieceBitboards[p])) {
        return true;
    }
    //attacked by knight
    if(bbState.knightAttack[tile] & ((side == White) ? bbState.pieceBitboards[N] : bbState.pieceBitboards[n])) {
        return true;
    }
    //attacked by bishop
    if(getBishopAttacks(tile, bbState.occupancies[Both]) & ((side == White) ? bbState.pieceBitboards[B] : bbState.pieceBitboards[b])) {
        return true;
    }
    //attacked by rook
    if(getRookAttacks(tile, bbState.occupancies[Both]) & ((side == White) ? bbState.pieceBitboards[R] : bbState.pieceBitboards[r])) {
        return true;
    }
    //attacked by queen
    if(getQueenAttacks(tile, bbState.occupancies[Both]) & ((side == White) ? bbState.pieceBitboards[Q] : bbState.pieceBitboards[q])) {
        return true;
    }
    //attacked by king
    if(bbState.kingAttack[tile] & ((side == White) ? bbState.pieceBitboards[K] : bbState.pieceBitboards[k])) {
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



