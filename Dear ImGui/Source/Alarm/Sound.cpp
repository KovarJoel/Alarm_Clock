#include "Sound.h"

Sound::Sound()
{
	m_soundEngine = irrklang::createIrrKlangDevice();
}

Sound::Sound(const std::string& path)
{
	m_soundEngine = irrklang::createIrrKlangDevice();
	setPath(path);
}

Sound::~Sound()
{
	m_soundEngine->drop();
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