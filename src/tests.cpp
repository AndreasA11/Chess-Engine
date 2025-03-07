#include "tests.hpp"
#include "movement.hpp"
#include "bitboard.hpp"
#include "consts.hpp"
#include "engine.hpp"

long Nodes = 0;

void PerfTests::perftDriver(int depth) {
    if(depth == 0) {
        ++Nodes;
        return;
    }

    Moves moveList;

    Movement::generateMoves(moveList);

    for(int count = 0; count < moveList.moveCount; ++count) {
        
        copy_board();
        

		if(!Movement::makeMove(moveList.moves[count], allMoves)) {
			continue;
		}
        
        PerfTests::perftDriver(depth - 1);
		
        take_back();
		
        
	}

     
}


void PerfTests::perfTest(int depth) {
    
    std::cout << "\n    Performance test\n\n";

    Moves moveList;

    Movement::generateMoves(moveList);

    auto begin  = Engine::getTimeInMS();

    for(int count = 0; count < moveList.moveCount; ++count) {
    
        copy_board();

		if(!Movement::makeMove(moveList.moves[count], allMoves)) {
			continue;
		}
        
        //cummulative Nodes
        //long cummulativeNodes = Nodes;

        PerfTests::perftDriver(depth - 1);
        

        //long oldNodes = Nodes - cummulativeNodes;
    
        take_back();

        // std::cout << "    move: "<< tileToCoord[getMoveSource(moveList->moves[count])] 
        //     << tileToCoord[getMoveTarget(moveList->moves[count])]
        //     << (getMovePromo(moveList->moves[count]) ? promotedPieces[getMovePromo(moveList->moves[count])] :  ' ') << "     Nodes:   " << oldNodes 
        //     << "    Move count: " << moveList->moveCount 
        //     << "    Count: " << count << "\n";
     
	}

    auto end = Engine::getTimeInMS();
	int duration = Engine::getTimeInterval(begin, end);

    std::cout << "\n\n    Depth: " << depth << "\n";
    std::cout << "    Nodes: " << Nodes << "\n";
    std::cout << "    Time: " << duration << "\n\n";
}