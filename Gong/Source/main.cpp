// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Application.h"

int main()
{
	Application app;
	if (!app.init("Gong", 1280, 720))
		return 1;

	while (!app.shouldClose())
	{
		app.handleEvents();
		app.update();
		app.render();
	}

	return 0;
}