#pragma once

#include "Time.h"

class Alarm
{
public:

	Time time;
	int enabled;	// for better compatibility with imgui widgets
	
	Alarm();
	Alarm(const Time& time);
	~Alarm() = default;

	void toggle();
	void toggle(bool enabled);
};