#pragma once

#include <iostream>
#include <vector>
#include <IrrKlang/irrKlang.h>

class Settings
{
private:
	static irrklang::ISoundEngine* m_soundEngine;

	static unsigned int m_styleColor;
	static float m_volume;
	static std::vector<std::string> m_soundFiles;
	static unsigned int m_soundFile;

public:
	Settings() = delete;
	static void renderStyle();
	static void renderSound();

private:
	static void soundVolume();
	static void soundFile();
	static void soundFileSelect();
	static void soundFileEdit();
	static void soundFileAdd();

	static bool fileExists(const std::string& filePath);
	static std::string copyFile(const std::string& source);
	static void renameFile(std::string& oldName, const std::string& newName);
	static void deleteFile(const std::string& name);
};