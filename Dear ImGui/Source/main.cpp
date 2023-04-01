#include "Application.h"

void main()
{
	Application app;
	app.init("Alarm Clock", 1270, 720);

	while (!app.shouldClose())
	{
		app.handleEvents();
		app.update();
		app.render();
	}
}