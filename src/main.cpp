#include "Interface.hpp"
#include <iostream>


int main(int argc, char *argv[]) {
	Interface *interface = nullptr;
	interface = new Interface();
	interface->init("GameWindow", 800, 640, false);

	while (interface->running())
	{
		interface->handleEvents();
		interface->render();
	}

	interface->clean();
	return 0;
}

