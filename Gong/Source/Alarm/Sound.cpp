// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Sound.h"

#include <filesystem>

Sound::Sound()
{
	extern irrklang::ISoundEngine* soundEngine;
	m_soundEngine = soundEngine;
}

Sound::Sound(const std::string& path)
{
	extern irrklang::ISoundEngine* soundEngine;
	m_soundEngine = soundEngine;

	setPath(path);
}

bool Sound::play()
{
	if (!std::filesystem::exists(m_path))
		return false;

	m_soundEngine->play2D(m_path.c_str());

	return true;
}

bool Sound::setPath(const std::string& path)
{
	if (!std::filesystem::exists(path))
		return false;

	m_path = path;
	return true;
}

void Sound::setEngine(irrklang::ISoundEngine* engine)
{
	m_soundEngine = engine;
}