#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "UCI.hpp"
#include "search.hpp"
#include "fen.hpp"
#include "consts.hpp"
int UCImanager::parseMove(std::string strIn) {
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

void UCImanager::parsePosition(std::string command) {
	//init iterator to the current character in the command string
	std::string tempStr = command.substr(9, 8);
	//parse UCI startpos command
	if(tempStr == "startpos") {
		//init chess board with start pos
		Fen::parseFEN(start_position);
	} else {
		//parse UCI "fen" command
		//make sure "fen" command is available within command string
		auto index = command.find("fen");
		if(index != std::string::npos) {
			//"fen" found
			index += 4;
			//init board position
			std::string positionStr = command.substr(index);
			Fen::parseFEN(positionStr);
		} else {
			//"fen not found", init starting position
			Fen::parseFEN(start_position);
		}
		
	}

	//parse moves if available after position
	auto moveIndex = command.find("moves");
	if(moveIndex != std::string::npos) {
		
		auto curIndex = moveIndex + 5;
		std::string moveString = command.substr(curIndex);
		//loop over moves within a move string 
		while(curIndex < command.size()) {
			++curIndex; //white space
			//parse next move
			moveString = command.substr(curIndex);
			std::cout << "moveString: " << moveString << "\n\n";
			int move = UCImanager::parseMove(moveString);
			if(move == 0) {
				break;
			}

			//make move on chess board
			Movement::makeMove(move, allMoves);
			//move cur character index to end of current move
			while(command[curIndex] != ' ' && curIndex < command.size()) {
				++curIndex;
			}

		}
	}
	BitBoard::printBoard();
}

void UCImanager::parseGo(std::string command) {
	int depth = -1;
	auto index = command.find("depth");
	if(index != std::string::npos) {
		std::string depthStr = command.substr(index + 6);
		depth = std::stoi(depthStr);
	} else {
		depth = 6;
	}
	// std::cout << "depth: " << depth << "\n";
	Search::searchPosition(depth);
}

void UCImanager::UCIloop() {
	//reset buffers
	std::cin.clear();
	std::cout.flush();

	//define user / GUI
	std::string input;
	

	//print engine info
	std::cout << "id name DCE\n";
	std::cout << "id name Andreas Andrade\n";
	std::cout << "uciok\n";
	//transposition table goes here*

	//main UCI loop
	while(true) {
		//reset user / GUi input
		input.resize(2000, 0);

		//make sure output reaches GUI
		std::cout.flush();

		std::cin >> input;
		//get user / GUI input
		if(!std::cin) {
			continue;
		}
		//make sure input is available 
		if(input[0] == '\n') {
			continue;
		}

		//parse UCI "isready" command
		auto index = input.find("isready");
		if(index != std::string::npos && index == 0) {
			std::cout << "readyok\n";
		} else if (index = input.find("position"), index != std::string::npos) {
			std::string temp;
			std::getline(std::cin, temp);
			input += temp;
			std::cout << "input: " << input << "\n";
			UCImanager::parsePosition(input);
		} else if (index = input.find("ucinewgame"), index != std::string::npos) {
			//parse UCI "ucinewgame" command
			UCImanager::parsePosition("position startpos");
		} else if (index = input.find("go"), index != std::string::npos) {
			//parse UCI go command
			std::string temp;
			std::getline(std::cin, temp);
			input += temp;
			UCImanager::parseGo(input);
		}
		//parse UCI "quit" command
		else if (index = input.find("quit"), index != std::string::npos) {
			break;
		} else if (index = input.find("uci"), index != std::string::npos) {
			//parse UCI "uci" command
			std::cout << "id name DCE\n";
			std::cout << "id name DreDre:)\n";
			std::cout << "uciok\n";
		}
	}

}