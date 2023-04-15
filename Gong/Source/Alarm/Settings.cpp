// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Settings.h"

#include <array>
#include <filesystem>
#include <ImGui/imgui.h>
#include <ImGui/imgui_stdlib.h>
#include <GLFW/glfw3.h>

irrklang::ISoundEngine* Settings::m_soundEngine;

unsigned int Settings::m_styleColor = 0;
float Settings::m_volume = 1.0f;
std::vector<std::string> Settings::m_soundFiles;
unsigned int Settings::m_file = 0;
unsigned int Settings::m_copyQueue = 0;

std::string Settings::getSoundFile()
{
	if (!m_soundFiles.size())
		return "";
	return (getDirectoryPath() + m_soundFiles.at(m_file));
}

void Settings::renderStyle()
{
	std::array<std::string, 2> colors = { "Style Colors Light", "Style Colors Dark" };
	std::string current = colors.at(m_styleColor);

	ImGui::SeparatorText("Style Color");

	ImGui::Text("Style Color");
	ImGui::SameLine();
	if (!ImGui::BeginCombo("##Style combo", current.c_str()))
		return;

	for (int i = 0; i < colors.size(); i++)
	{
		bool isSelected = (i == m_styleColor);
		if (ImGui::Selectable(colors[i].c_str(), isSelected))
			m_styleColor = i;
	}
	ImGui::EndCombo();

	static unsigned int lastColor = 0;
	if (lastColor == m_styleColor)
		return;

	switch (m_styleColor)
	{
	case 1:
		ImGui::StyleColorsDark();
		break;
	default:
		ImGui::StyleColorsLight();
		break;
	}
	lastColor = m_styleColor;
}

void Settings::renderSound()
{
	extern irrklang::ISoundEngine* soundEngine;
	m_soundEngine = soundEngine;
	
	ImGui::SeparatorText("Sound");

	soundVolume();
	soundFile();
}

void Settings::soundVolume()
{
	m_volume = m_soundEngine->getSoundVolume() * 100.0f;

	if (m_volume <= 0.0f)
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Volume     ");
	else
		ImGui::Text("Volume     ");
	ImGui::SameLine();
	ImGui::SliderFloat("##Volume slider", &m_volume, 0.0f, 100.0f, "%.0f%%");
	m_soundEngine->setSoundVolume(m_volume / 100.0f);
}

void Settings::soundFile()
{
	soundFileSelect();
	soundFileEdit();
	soundFileAdd();
}

void Settings::soundFileSelect()
{
	pullDirectoryFiles();

	if (m_copyQueue)
	{
		static double first = glfwGetTime();
		if (glfwGetTime() > first + 1.0)	// after one second of no change, stop the queue
			m_copyQueue = 0;

		if (m_soundFiles.size() > m_copyQueue)
		{
			m_file++;
			m_copyQueue = 0;
		}
	}

	std::string preview = (m_soundFiles.size()) ? m_soundFiles.at(m_file) : "NONE";

	if (preview == "NONE")
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Sound File ");
	else
		ImGui::Text("Sound File ");
	ImGui::SameLine();

	if (ImGui::BeginCombo("##Sound combo", preview.c_str())) {
		for (int i = 0; i < m_soundFiles.size(); i++)
		{
			bool isSelected = (i == m_file);
			if (ImGui::Selectable(m_soundFiles[i].c_str(), isSelected))
				m_file = i;
		}
		ImGui::EndCombo();
	}
}

void Settings::soundFileEdit()
{
	if (!m_soundFiles.size())
		return;

	ImGui::SameLine();
	if (ImGui::Button("Delete")) {
		std::filesystem::remove(getDirectoryPath() + m_soundFiles.at(m_file));
		if (m_file)
			m_file--;
	}

	static std::string newName;
	ImGui::Text("           ");
	ImGui::SameLine();
	ImGui::InputTextWithHint("##rename sound", "new name (keep same extension)", &newName);
	ImGui::SameLine();
	if (ImGui::Button("Rename")) {
		if (getExtension(m_soundFiles.at(m_file)) != getExtension(newName))
			return;
		if (newName.size() <= getExtension(m_soundFiles.at(m_file)).size())
			return;

		renameFile(m_soundFiles.at(m_file), newName);

		pullDirectoryFiles();
		for (unsigned int i = 0; i < m_soundFiles.size(); i++) {
			if (m_soundFiles.at(i) == newName) {
				m_file = i;
				break;
			}
		}

		newName.clear();
	}
}

void Settings::soundFileAdd()
{
	static std::string newSound;
	ImGui::Text("           ");
	ImGui::SameLine();
	ImGui::InputTextWithHint("##add sound", "e.g. C:\\Users\\Max\\Desktop\\sound.wav", &newSound);
	ImGui::SameLine();
	if (ImGui::Button(" Add  ")) {

		if (std::filesystem::exists(newSound))
		{
			copyFile(newSound);
			newSound.clear();
			m_copyQueue = m_soundFiles.size();
		}
	}
}

void Settings::copyFile(const std::string& source)
{
	std::string name = getFileNameSubstr(source);
	std::string extension = getExtension(name);

	name = name.substr(0, name.find('.', 0));
	while (std::filesystem::exists(getDirectoryPath() + name + extension))
	{
		if (!std::isdigit(name.back())) {
			name += "1";
		}
		else {
			size_t first = name.find_last_not_of("0123456789", name.size()) + 1;
			size_t last = name.find_last_of("0123456789", name.size()) + 1;
			int value = atoi(name.substr(first, last - first).c_str());
			value++;

			name = name.substr(0, first);
			name += std::to_string(value);
		}
	}

	std::filesystem::copy(source, getDirectoryPath() + name + extension);
}

void Settings::renameFile(std::string& oldName, const std::string& newName)
{
	std::filesystem::rename(getDirectoryPath() + oldName, getDirectoryPath() + newName);
	oldName = newName;
}

void Settings::pullDirectoryFiles()
{
	m_soundFiles.clear();
	for (const auto& entry : std::filesystem::directory_iterator(getDirectoryPath()))
	{
		std::stringstream buffer;
		buffer << entry.path();
		m_soundFiles.emplace_back(getFileNameSubstr(buffer.str().substr(0, buffer.str().size() - 1)));	// remove trailing "
	}
}

std::string Settings::getDirectoryPath()
{
	char* buff;
	_dupenv_s(&buff, nullptr, "AppData");
	if (!buff)
		return "";
	std::string folderPath = buff;
	free(buff);
	folderPath += "\\Gong\\";
	return folderPath;
}

std::string Settings::getFileNameSubstr(const std::string& name)
{
	return name.substr(name.find_last_of('\\') + 1, name.size() - name.find_last_of('\\') + 1);
}

std::string Settings::getExtension(const std::string& name)
{
	if (name.find('.', 0) == std::string::npos)
		return "";
	return name.substr(name.find('.', 0), name.size() - name.find('.', 0));
}