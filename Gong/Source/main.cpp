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