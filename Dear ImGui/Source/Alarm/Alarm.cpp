#include "Alarm.h"

Alarm::Alarm()
	: time(Time::now()), enabled(false) {
}

Alarm::Alarm(const Time& time)
	: time(time), enabled(true) {
}

void Alarm::toggle()
{
	enabled = !enabled;
}

void Alarm::toggle(bool enabled)
{
	this->enabled = enabled;
}