#include "Interface.hpp"
#include <iostream>
#include <string>
#include <SDL2/SDL.h>

SDL_Renderer* Interface::renderer = nullptr;
SDL_Event Interface::event;
bool Interface::isRunning = false;


void Interface::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
		}

		isRunning = true;
	}


}


void Interface::handleEvents()
{
	
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}
}


void Interface::render()
{
	SDL_RenderClear(renderer);
	

	SDL_RenderPresent(renderer);
}

void Interface::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}