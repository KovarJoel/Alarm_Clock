#pragma once

#include <Windows.h>
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>
#include <algorithm>
#include "Alarm.h"
#include "Sound.h"
#include "Console.h"

class Application
{
private:
	std::vector<Alarm> alarms;
	Sound sound;
	
	std::thread ring;
	void ringCallback();

	void sortAlarms();

	void setupConsole(const std::string& windowTitle);
	
public:
	Application(const std::string& windowTitle);
	~Application();

	bool isRunning;

	void handleEvents();
	void render();
};