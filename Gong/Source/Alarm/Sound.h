#pragma once

#include <iostream>
#include <irrKlang/irrKlang.h>

class Sound
{
private:
	std::string m_path;
	irrklang::ISoundEngine* m_soundEngine;

public:

	Sound();
	Sound(const std::string& path);
	~Sound() = default;
	
	bool play();
	bool setPath(const std::string& path);
	void setEngine(irrklang::ISoundEngine* engine);
};