#include "Alarm.h"

Alarm::Alarm()
	: time(Time()), enabled(false) {

	COORD coord = { -10, -10 };
	button = std::make_shared<Button>("", coord);
}

Alarm::Alarm(const Time& time)
	: time(time), enabled(true) {

	COORD coord = { -10, -10 };
	button = std::make_shared<Button>("", coord);
}

void Alarm::toggle()
{
	enabled = !enabled;
}

void Alarm::toggle(bool enabled)
{
	this->enabled = enabled;
}