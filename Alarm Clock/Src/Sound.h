#pragma once

#include <iostream>
#include <Windows.h>
#include <fstream>

class Sound
{
public:
	std::string path;

	Sound() = default;
	Sound(const std::string& path);
	
	bool play();
	bool setPath(const std::string& path);
};