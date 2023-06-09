// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "AlarmClock.h"

#include "Settings.h"
#include "SavedValues.h"

#include <array>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_stdlib.h>

AlarmClock::AlarmClock()
	:m_size(900, 506), m_position(200, 100) {
}

AlarmClock::~AlarmClock()
{
	close();
}

void AlarmClock::init()
{
	if (m_isRunning)
		return;
	extern GLFWwindow* window;
	m_window = window;
	
	extern irrklang::ISoundEngine* soundEngine;
	m_sound.setEngine(soundEngine);

	SavedValues::createFile();
	Settings::init();

	alarms = SavedValues::loadAlarms();
	sortAlarms();
	
	bool alreadyHasFile = false;
	for (auto& entry : std::filesystem::directory_iterator(Settings::getDirectoryPath())) {
		if (!Settings::getExtension(Settings::getFileNameSubstr(entry.path().string())).empty()) {
			alreadyHasFile = true;
			break;
		}
	}
	if (!alreadyHasFile)
		std::filesystem::copy_file(std::filesystem::current_path().string() + "\\Dependencies\\Sounds\\gong.wav", Settings::getDirectoryPath() + "gong.wav");

	m_isRunning = true;
}

void AlarmClock::handleEvents()
{

}

void AlarmClock::update()
{
	glfwSetWindowSize(m_window, (int)m_size.x, (int)m_size.y);
	glfwSetWindowPos(m_window, (int)m_position.x, (int)m_position.y);

	if (!m_isRunning)
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);

	m_sound.setPath(Settings::getSoundFile());
	ring();
}

void AlarmClock::render()
{	
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoResize;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize;

	if (!ImGui::Begin("Gong", &m_isRunning, flags))
		return;

	static bool doOnce = true;
	if (doOnce) {
		ImGui::SetWindowSize(m_size);
		ImGui::SetWindowPos(m_position);
		doOnce = false;
	}
	if (ImGui::BeginTabBar("Bar"))
	{
		if (ImGui::BeginTabItem("Alarms")) {
			renderAlarms();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Settings")) {
			renderSettings();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void AlarmClock::renderAlarms()
{
	ImGui::Text(Time::toString(Time::now()).c_str());
	ImGui::NewLine();

	ImGui::SeparatorText("Upcoming Alarms");

	static int child = -1;
	bool changes = false;
	for (int i = 0; i < alarms.size(); i++)
	{
		Alarm& alarm = alarms[i];

		std::string hiddenID = "##" + std::to_string(i);
		ImGui::Text(Time::toString(alarm.time).c_str());

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		ImGui::SliderInt((hiddenID + " toggle ").c_str(), &alarm.enabled, 0, 1, (alarm.enabled ? "ON" : "OFF"));

		ImGui::SameLine();
		if (ImGui::Button(("EDIT" + hiddenID).c_str()))
			child = i;

		ImGui::SameLine();
		if (ImGui::Button(("DELETE" + hiddenID).c_str()))
		{
			alarms.erase(alarms.begin() + i);

			if (child == i)
				child = -1;
			if (child > i)
				child--;
			changes = true;
		}

		if (child == i)
			renderAlarmsChild(child);
	}

	ImGui::NewLine();
	ImGui::NewLine();
	if (ImGui::Button("Add Alarm"))
	{
		sortAlarms();

		alarms.emplace_back(Alarm(Time::last()));
		child = (int)alarms.size() - 1;
		changes = true;
	}
	ImGui::NewLine();

	m_size = ImGui::GetWindowSize();
	m_position = ImGui::GetWindowPos();

	if (changes)
		SavedValues::saveAlarms(alarms);
}

void AlarmClock::renderAlarmsChild(int& child)
{
	float width = 125.0f;
	Time& time = alarms[child].time;

	ImGui::BeginChild(ImGui::GetID("Alarms"), ImVec2(0, ImGui::GetFontSize() * 4.0f), false);
	ImGui::SetNextItemWidth(width);
	ImGui::SliderInt("##edit hours", &time.hours, 0, 23);
	ImGui::SameLine(0.0f, 4.0f);
	ImGui::SetNextItemWidth(width * 2.0f + 4.0f);
	ImGui::SliderInt2("##edit rest", &time.minutes, 0, 59);
	ImGui::SameLine(0.0f, 4.0f);
	ImGui::SetNextItemWidth(width);
	ImGui::SliderInt("##edit day", &time.day, 0, 6, Time::weekDayToString(time.day).c_str());

	if (ImGui::Button("Done"))
	{
		sortAlarms();
		child = -1;
		SavedValues::saveAlarms(alarms);
	}
	ImGui::EndChild();
}

void AlarmClock::renderSettings()
{
	Settings::renderStyle();
	ImGui::NewLine();
	Settings::renderSound();
}

void AlarmClock::close()
{
	m_isRunning = false;
}

void AlarmClock::sortAlarms()
{
	Alarm temp;
	bool changes = false;

	if (!alarms.size())
		return;

	do
	{
		changes = false;

		for (size_t i = 0; i < alarms.size() - 1; i++) {
			if (alarms.at(i).time > alarms.at(i + 1).time) {
				temp = alarms.at(i);
				alarms.at(i) = alarms.at(i + 1);
				alarms.at(i + 1) = temp;

				changes = true;
			}
		}
	} while (changes);
}

void AlarmClock::sortAlarmsUpcoming()
{
	if (!alarms.size())
		return;

	std::vector<Alarm> tempAlarms = alarms;
	Time now = Time::now();
	size_t next = 0;

	for (size_t i = 0; i < alarms.size(); i++) {
		if (alarms[i].time >= now) {
			next = i;
			break;
		}
	}

	for (size_t i = next, j = 0; j < alarms.size(); i++, j++)
	{
		if (i < alarms.size())
			alarms[j] = tempAlarms[i];
		else
			alarms[j] = tempAlarms[i - alarms.size()];
	}
}

void AlarmClock::ring()
{
	using namespace std::chrono_literals;
	
	Time now = Time::now();
	static Time lastRing;
	
	if (now == lastRing)
		return;

	for (auto& alarm : alarms)
	{
		if (!alarm.enabled)
			continue;
		if (!(now == alarm.time))
			continue;

		m_sound.play();
		lastRing = now;
	}
}