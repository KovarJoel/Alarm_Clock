#include "Application.h"

void main()
{
	Application app("Alarm Clock");

	while (app.isRunning)
	{
		app.handleEvents();
		app.render();
	}
}