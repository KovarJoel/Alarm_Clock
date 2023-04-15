// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Alarm.h"

Alarm::Alarm()
	: time(Time::now()), enabled(true) {
}

Alarm::Alarm(const Time& time)
	: time(time), enabled(true) {
}

void Alarm::toggle()
{
	if (enabled)
		enabled = 0;
	else
		enabled = 1;
}

void Alarm::toggle(bool enabled)
{
	this->enabled = enabled;
}