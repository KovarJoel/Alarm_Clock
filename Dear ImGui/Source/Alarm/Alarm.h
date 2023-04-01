#pragma once

#include <ImGui/imgui.h>
#include "Time.h"

class Alarm
{
public:

	Time time;
	bool enabled;
	
	Alarm();
	Alarm(const Time& time);
	~Alarm() = default;

	void toggle();
	void toggle(bool enabled);
};