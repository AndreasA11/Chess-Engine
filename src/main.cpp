#include <iostream>
#include "bitboard.hpp"
#include "movement.hpp"
#include "engine.hpp"
#include "tests.hpp"
#include "fen.hpp"
#include "UCI.hpp"
#include "search.hpp"

/*
const std::string empty_board = "8/8/8/8/8/8/8/8 w - - ";
const std::string start_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";
const std::string tricky_position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
const std::string killer_position = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
const std::string cmk_position = "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";
const std::string position6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ";
*/


/***********************
  --------------------
	  DRIVER BEGIN
  --------------------
***********************/

int main(int argc, char *argv[]) {

	
	BitBoard::initLeaperAttacks();
	BitBoard::initSliderAttacks(bishop);
	BitBoard::initSliderAttacks(rook);
	
	
	int debug = 1;

	if(debug) {
		Fen::parseFEN(tricky_position);
		BitBoard::printBoard();
		Search::searchPosition(6);
		BitBoard::printBoard();
		//BitBoard::printBoard();



	} else {
		UCImanager::UCIloop();
	}

	return 0;

}

/***********************
  --------------------
	   DRIVER END
  --------------------
***********************/











