#include "fen.hpp"
#include <iostream>

void fen::parseFEN(std::string fen) {
    memset(BitBoard::bbState.pieceBitboards, ZERO, sizeof(BitBoard::bbState.pieceBitboards));
    memset(BitBoard::bbState.occupancies, ZERO, sizeof(BitBoard::bbState.occupancies));
    BitBoard::bbState.side = White;
    BitBoard::bbState.enpassant = noTile;
    BitBoard::bbState.canCastle = 0;
    int count = 0;
    for(int rank = 0; rank < 8; ++rank) {
        for(int file = 0; file < 8; ++file) {
            //init current tile
            int tile = rank * 8 + file;
            if(((fen[count] >= 'a') && (fen[count] <= 'z')) || ((fen[count] >= 'A') && (fen[count] <= 'Z'))) {
                //init piece type
                int out = fen[count];
                int piece = charPieces[out];
                setBitOn(BitBoard::bbState.pieceBitboards[piece], tile);
                ++count;
            } 
            //empty tiles
            if(fen[count] >= '0' && fen[count] <= '9') {
                int offset = fen[count] - '0';
                int piece = -1;
                for(int bbPiece = P; bbPiece <= k; ++bbPiece) {
                    //if there is a piece on current tile the we get the piece code
                    if(getBit(BitBoard::bbState.pieceBitboards[bbPiece], tile)) {
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
        BitBoard::bbState.side = White;
    } else {
        BitBoard::bbState.side = Black;
    }
    count += 2;
    //parse castling
    while(fen[count] != ' ') {
        switch(fen[count]) {
            case 'K':
                BitBoard::bbState.canCastle |= WKCA;
                break;
            case 'Q':
                BitBoard::bbState.canCastle |= WQCA;
                break;
            case 'k':
                BitBoard::bbState.canCastle |= BKCA;
                break;
            case 'q':
                BitBoard::bbState.canCastle |= BQCA;
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
        BitBoard::bbState.enpassant = rank * 8 + file;
    } else { //no enpassant
        BitBoard::bbState.enpassant = noTile;
    }
    //loop over white pieces bitboards
    for(int piece = P; piece <= K; ++piece) {
        BitBoard::bbState.occupancies[White] |= BitBoard::bbState.pieceBitboards[piece];
    }
    for(int piece = p; piece <= k; ++piece) {
        BitBoard::bbState.occupancies[Black] |= BitBoard::bbState.pieceBitboards[piece];
    }
    BitBoard::bbState.occupancies[Both] |= (BitBoard::bbState.occupancies[White] | BitBoard::bbState.occupancies[Black]);
    std::cout << "fen: " << fen << "\n";
}


