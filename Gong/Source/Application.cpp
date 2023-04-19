// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Application.h"

#include <string>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

GLFWwindow* window = nullptr;
irrklang::ISoundEngine* soundEngine = nullptr;

Application::Application()
	: m_initializedGLFW(false), m_initializedDearImGui(false), m_window(nullptr), m_soundEngine(nullptr) {
}

Application::Application(const std::string& windowTitle, const int width, const int height)
	: m_initializedGLFW(false), m_initializedDearImGui(false), m_window(nullptr), m_soundEngine(nullptr)
{
	init(windowTitle, width, height);
}

Application::~Application()
{
	m_alarmClock.close();

	if (m_initializedDearImGui)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	
	if (m_initializedGLFW)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}

bool Application::init(const std::string& windowTitle, const int width, const int height)
{
	if (m_initializedGLFW || m_initializedDearImGui)
		return false;

	initGLFW(windowTitle, width, height);
	if (!m_initializedGLFW)
		return false;

	initDearImGui();
	if (!m_initializedDearImGui)
		return false;

	m_soundEngine = irrklang::createIrrKlangDevice();
	if (!m_soundEngine)
		return false;
	m_soundEngine->setSoundVolume(1.0f);
	soundEngine = m_soundEngine;

	m_alarmClock.init();

	return true;
}

void Application::initGLFW(const std::string& windowTitle, const int width, const int height)
{
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	// make window transparent without borders
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	m_window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
	if (!m_window)
		return;
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);	// v-sync

	window = m_window;
	m_initializedGLFW = true;
}

void Application::initDearImGui()
{
	std::string glsl_version = "#version " + std::to_string(GLFW_VERSION_MAJOR) + std::to_string(GLFW_VERSION_MINOR) + "0";

	if (!IMGUI_CHECKVERSION())
		return;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsLight();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	if (!ImGui_ImplGlfw_InitForOpenGL(m_window, true))
		return;
	if (!ImGui_ImplOpenGL3_Init(glsl_version.c_str()))
		return;

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {glfwSetWindowShouldClose(window, GL_TRUE); });
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height); });

	ImGui::GetIO().Fonts->AddFontFromFileTTF("Dependencies\\Fonts\\FiraCode-Regular.ttf", 24.0f);

	m_initializedDearImGui = true;
}

bool Application::shouldClose() const
{
	if (!m_initializedGLFW || !m_initializedDearImGui)
		return true;
	return glfwWindowShouldClose(m_window);
}

void Application::handleEvents()
{
	glfwPollEvents();
	
	// only use events if imgui isnt
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard)
		return;
	if (io.WantCaptureMouse)
		return;

	m_alarmClock.handleEvents();
}

void Application::update()
{
	m_alarmClock.update();
}

void Application::render()
{
	renderStart();

	m_alarmClock.render();
	ImGui::ShowDemoWindow();

	renderEnd();
}

void Application::renderStart()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Application::renderEnd()
{
	ImGui::Render();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	glfwSwapBuffers(m_window);
}