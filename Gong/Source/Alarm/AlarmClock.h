#pragma once

#include <ImGui/imgui.h>
#include <GLFW/glfw3.h>
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

	const GLFWwindow* window = nullptr;

	ImVec2 size;
	ImVec2 position;
	
	std::thread ring;
	void ringCallback();

	void sortAlarms();
	void sortAlarmsUpcoming();
	
public:
	AlarmClock();
	~AlarmClock();
	void init(GLFWwindow* window);

	std::vector<Alarm> alarms;

	void handleEvents();
	void update();
	void render();

	void close();
};