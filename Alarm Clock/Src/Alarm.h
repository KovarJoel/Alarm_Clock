#pragma once

#include "Time.h"
#include "Button.h"

class Alarm
{
public:

	Time time;
	bool enabled;
	
	std::shared_ptr<Button> button;

	Alarm();
	Alarm(const Time& time);
	~Alarm() = default;

	void toggle();
	void toggle(bool enabled);
};