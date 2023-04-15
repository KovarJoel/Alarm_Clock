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
	bool m_isRunning = false;
	Sound m_sound;

	GLFWwindow* m_window = nullptr;

	ImGuiID dockSpace = 0;

	ImVec2 m_size;
	ImVec2 m_position;
	
	std::thread m_ring;

public:
	std::vector<Alarm> alarms;
	
public:
	AlarmClock();
	~AlarmClock();
	void init();

	void handleEvents();
	void update();
	void render();

	void close();

private:
	void renderAlarms();
	void renderAlarmsChild(int& child);

	void renderSettings();

private:
	void ringCallback();

	void sortAlarms();
	void sortAlarmsUpcoming();
};