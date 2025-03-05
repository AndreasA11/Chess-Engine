#include <iostream>
#include "Interface.hpp"
#include "bitboard.hpp"
#include "movement.hpp"
//DRIVER
std::string empty_board = "8/8/8/8/8/8/8/8 w - - ";
std::string start_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";
std::string tricky_position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
std::string killer_position = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
std::string cmk_position = "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";


int main(int argc, char *argv[]) {
	// Interface *interface = nullptr;
	// interface = new Interface();
	// interface->init("GameWindow", 800, 640, false);

	// while (interface->running())
	// {
	// 	interface->handleEvents();
	// 	interface->render();
	// }

	// interface->clean();
	

	BitBoard::parseFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ");
	BitBoard::initLeaperAttacks();
	BitBoard::initSliderAttacks(bishop);
	BitBoard::initSliderAttacks(rook);
	
	BitBoard::printBoard();
	Movement::generateMoves();
	Movement::addMove(encodeMove(d7, e8, B, q, 1, 0, 1, 0));
	Movement::printMoveList();
	
	
	
	
	return 0;

}

