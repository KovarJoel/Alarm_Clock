#include "Application.h"

Application::~Application()
{
	if (!m_isInitialized)
	{
		Assert::assertMsg("Application is not initialized!");
		return;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Application::init(const std::string& windowTitle, const int width, const int height)
{
	if (!glfwInit())
		return false;

	std::string glsl_version = "#version " + std::to_string(GLFW_VERSION_MAJOR) + std::to_string(GLFW_VERSION_MINOR) + "0";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	
	m_window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
	if (!m_window)
		return false;
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);	//vsync

	if (!IMGUI_CHECKVERSION())
		return false;
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
		return false;
	if (!ImGui_ImplOpenGL3_Init(glsl_version.c_str()))
		return false;

	auto closeCallback = [](GLFWwindow* window) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	};
	glfwSetWindowCloseCallback(m_window, closeCallback);
	
	glfwSetErrorCallback(Assert::assertGlfwCallback);

	m_clock.init(m_window);

	ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\FiraCode-Regular.ttf", 20.0f);

	m_isInitialized = true;
	return true;
}

bool Application::shouldClose() const
{
	if (!m_isInitialized)
		return true;
	return glfwWindowShouldClose(m_window);
}

void Application::handleEvents()
{
	glfwPollEvents();

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard)
		return;
	if (io.WantCaptureMouse)
		return;

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, GL_TRUE);
}

void Application::update()
{
	m_clock.update();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Application::render()
{
	m_clock.render();

	//ImGui::ShowDemoWindow();

	renderEnd();
}

void Application::renderEnd()
{
	ImGui::Render();

	int display_w, display_h;
	glfwGetFramebufferSize(m_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

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