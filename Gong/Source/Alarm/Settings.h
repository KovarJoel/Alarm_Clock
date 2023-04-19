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
	static unsigned int m_file;
	static unsigned int m_copyQueue;	// increment m_file not immediately after starting to copy but rather on completion

public:
	Settings() = delete;
	static void init();
	static std::string getSoundFile();
	static void renderStyle();
	static void renderSound();

	static std::string getDirectoryPath();
	static std::string getFileNameSubstr(const std::string& name);
	static std::string getExtension(const std::string& name);

private:
	static void soundVolume();
	static void soundFile();
	static void soundFileSelect();
	static void soundFileEdit();
	static void soundFileAdd();

	static void copyFile(const std::string& source);
	static void renameFile(std::string& oldName, const std::string& newName);

	static void pullDirectoryFiles();
};