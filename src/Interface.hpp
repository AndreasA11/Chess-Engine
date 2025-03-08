#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

namespace Interface {

	
	//parse input move string (e.g "e7e8q")
	int parseMove(std::string strIn);

};

#endif //INTERFACE_H