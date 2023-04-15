// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Settings.h"

#include <array>
#include <fstream>
#include <ImGui/imgui.h>
#include <ImGui/imgui_stdlib.h>

irrklang::ISoundEngine* Settings::m_soundEngine;

unsigned int Settings::m_styleColor = 0;
float Settings::m_volume = 1.0f;
std::vector<std::string> Settings::m_soundFiles = { "gong.wav" };
unsigned int Settings::m_soundFile = 0;

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
		if (ImGui::Selectable(colors.at(i).c_str(), isSelected))
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
	std::string current = m_soundFiles.at(m_soundFile);

	ImGui::Text("Sound File ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##Sound combo", current.c_str())) {
		for (int i = 0; i < m_soundFiles.size(); i++)
		{
			bool isSelected = (i == m_soundFile);
			if (ImGui::Selectable(m_soundFiles.at(i).c_str(), isSelected))
				m_soundFile = i;
		}
		ImGui::EndCombo();
	}
}

void Settings::soundFileEdit()
{
	ImGui::SameLine();
	if (ImGui::Button("Delete")) {
		deleteFile(m_soundFiles.at(m_soundFile));
		m_soundFiles.erase(m_soundFiles.begin() + m_soundFile);
		m_soundFile--;
	}

	static std::string newName;
	ImGui::Text("           ");
	ImGui::SameLine();
	ImGui::InputTextWithHint("##rename sound", "new name", &newName);
	ImGui::SameLine();
	if (ImGui::Button("Rename")) {
		renameFile(m_soundFiles.at(m_soundFile), newName);
		newName = "";
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

		if (fileExists(newSound))
		{
			m_soundFiles.push_back(copyFile(newSound));
			m_soundFile = (int)m_soundFiles.size() - 1;
			newSound = "";
		}
	}
}

bool Settings::fileExists(const std::string& filePath)
{
	if (filePath.empty())
		return false;

	std::ifstream file(filePath);

	if (file.good())
	{
		file.close();
		return true;
	}

	file.close();
	return false;
}

std::string Settings::copyFile(const std::string& source)
{
	char* buff;
	_dupenv_s(&buff, nullptr, "AppData");
	if (!buff)
		return "";
	std::string folderPath = buff;
	free(buff);

	folderPath += "\\Gong\\";
	std::string name = source.substr(source.find_last_of('\\') + 1, source.size() - source.find_last_of('\\') + 1);
	std::string extension = name.substr(name.find('.', 0), name.size() - name.find('.', 0));
	
	if (fileExists(folderPath + name)) {
		name = name.substr(0, name.find('.', 0));

		if (!std::isdigit(name.back())) {
			name += "1";
		}
		else
		{
			size_t first = name.find_last_not_of("0123456789", name.size()) + 1;
			size_t last = name.find_last_of("0123456789", name.size()) + 1;
			int value = atoi(name.substr(first, last - first).c_str());
			value++;

			name = name.substr(0, first);
			name += std::to_string(value);
		}
		name += extension;
	}

	std::ifstream src;
	std::ofstream dest;
	src.open(source, std::ios::binary);
	dest.open(folderPath + name, std::ios::binary);

	dest << src.rdbuf();

	dest.close();
	src.close();

	return name;
}

void Settings::renameFile(std::string& oldName, const std::string& newName)
{
	char* buff;
	_dupenv_s(&buff, nullptr, "AppData");
	if (!buff)
		return;
	std::string folderPath = buff;
	free(buff);
	folderPath += "\\Gong\\";

	(void)rename((folderPath + oldName).c_str(), (folderPath + newName).c_str());

	oldName = newName;
}

void Settings::deleteFile(const std::string& name)
{
	char* buff;
	_dupenv_s(&buff, nullptr, "AppData");
	if (!buff)
		return;
	std::string folderPath = buff;
	free(buff);
	folderPath += "\\Gong\\";

	(void)(remove((folderPath + name).c_str()));
}