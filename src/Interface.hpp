#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

class Interface
{
public:

	void init(const char* title, int width, int height, bool fullscreen);
	
	void handleEvents();
	bool running() { return isRunning; }
	void render();
	void clean();

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static bool isRunning;

private:

	//int count = 0;
	SDL_Window *window;
};