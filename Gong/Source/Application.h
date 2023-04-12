#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <irrKlang/irrKlang.h>
#include <ImGui/imgui.h>

class Application
{
private:
	bool m_initializedGLFW, m_initializedDearImGui;

	GLFWwindow* m_window;
	irrklang::ISoundEngine* m_soundEngine;

public:
	Application();
	Application(const std::string& windowTitle, const int width, const int height);
	~Application();

	bool init(const std::string& windowTitle, const int width, const int height);
	bool shouldClose() const;

	void handleEvents();
	void update();
	void render();

private:
	void initGLFW(const std::string& windowTitle, const int width, const int height);
	void initDearImGui();

	void renderStart();
	void renderEnd();
};