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
        pawnAttack[White][tile] = maskPawnAttacks(tile, Black);
        pawnAttack[White][tile] = maskPawnAttacks(tile, Black);
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
        std::cout << "Enpassant: " << tileToCoord[enpassant] << "\n";
    } else {
        std::cout << "Enpassant: " <<  "no\n\n";
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
