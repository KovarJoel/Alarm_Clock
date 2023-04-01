#include "Sound.h"

Sound::Sound(const std::string& path)
{
	setPath(path);
}

bool Sound::play()
{
	if (path.empty())
		return false;

	std::string command = "start " + path;
	system(command.c_str());
	return true;
}

bool Sound::setPath(const std::string& path)
{
	std::ifstream file(path);
	
	if (!file.good())
		return false;

	this->path = path;
	return true;
}