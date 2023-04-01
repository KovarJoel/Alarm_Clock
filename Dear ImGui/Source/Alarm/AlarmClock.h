#pragma once

#include <ImGui/imgui.h>
#include <vector>
#include <thread>
#include <chrono>
#include "Alarm.h"
#include "Sound.h"

class AlarmClock
{
private:
	bool isRunning = false;
	Sound sound;
	
	std::thread ring;
	void ringCallback();

	void sortAlarms();
	void sortAlarmsUpcoming();
	
public:
	AlarmClock();
	~AlarmClock();
	void init();

	std::vector<Alarm> alarms;

	void handleEvents();
	void update();
	void render();

	void close();
};