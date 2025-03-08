#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "interface.hpp"
#include "bitboard.hpp"
#include "movement.hpp"

int Interface::parseMove(std::string strIn) {
	Moves moveList;
	Movement::generateMoves(moveList);

	int sourceTile = (strIn[0] - 'a') + (8 - (strIn[1] - '0')) * 8;
	int targetTile = (strIn[2] - 'a') + (8 - (strIn[3] - '0')) * 8;	
	//loop over moves in a move list
	for(int count = 0; count < moveList.moveCount; ++count) {
		int move = moveList.moves[count];
		//source and target tile are available in the move list
		if(sourceTile == getMoveSource(move) && targetTile == getMoveTarget(move)) {
			int promoPiece = getMovePromo(move);
			if(promoPiece) {
				if((promoPiece == Q || promoPiece == q) && strIn[4] == 'q') {
					return move; //queen promotion
				} else if ((promoPiece == R || promoPiece == r) && strIn[4] == 'r') {
					return move; //rook promotion
				} else if ((promoPiece == B || promoPiece == b) && strIn[4] == 'b') {
					return move; //bishop promotion
				} else if((promoPiece == N || promoPiece == n) && strIn[4] == 'n') {
					return move; //knight promotion
				} else {
					continue;
				}
			}
			return move;
		}
	}
	//illegal move
	return 0;
}