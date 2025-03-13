#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>


namespace UCImanager {


	//parse input move string (e.g "e7e8q")
	int parseMove(std::string strIn);
	//parse UCI position command
	void parsePosition(std::string command);
	//parse UCI "go" command
	void parseGo(std::string command);
	
	void UCIloop();
};

#endif //INTERFACE_H