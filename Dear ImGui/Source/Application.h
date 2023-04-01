#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_glfw.h>

#include "Assert.h"
#include "Alarm/AlarmClock.h"

class Application
{
private:
	bool m_isInitialized = false;
	GLFWwindow* m_window = nullptr;
	ImColor m_clearColor{};
	AlarmClock m_clock;

	void renderEnd();

public:
	Application() = default;
	~Application();
	
	bool init(const std::string& windowTitle, const int width, const int height);
	bool shouldClose() const;
	
	void handleEvents();
	void update();
	void render();
};

