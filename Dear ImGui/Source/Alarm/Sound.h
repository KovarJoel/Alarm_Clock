#pragma once

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <irrKlang/irrKlang.h>

class Sound
{
private:
	std::string m_path;
	irrklang::ISoundEngine* m_soundEngine;

public:

	Sound();
	Sound(const std::string& path);
	~Sound();
	
	bool play();
	bool setPath(const std::string& path);

private:
	bool pathOk(const std::string& path);
};