#include "Sound.h"

#include <fstream>

Sound::Sound()
{
	extern irrklang::ISoundEngine* soundEngine;
	m_soundEngine = m_soundEngine;
}

Sound::Sound(const std::string& path)
{
	extern irrklang::ISoundEngine* soundEngine;
	m_soundEngine = m_soundEngine;

	setPath(path);
}

bool Sound::play()
{
	if (!pathOk(m_path))
		return false;

	m_soundEngine->play2D(m_path.c_str());

	return true;
}

bool Sound::setPath(const std::string& path)
{
	if (!pathOk(path))
		return false;

	m_path = path;
	return true;
}

bool Sound::pathOk(const std::string& path)
{
	if (path.empty())
		return false;

	std::ifstream file(path);

	if (file.good())
		return true;
	return false;
}