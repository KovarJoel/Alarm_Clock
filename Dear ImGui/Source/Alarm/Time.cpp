#include "Time.h"

Time::Time()
	: seconds(0), minutes(0), hours(0), day(Sunday) {
}

Time::Time(int day, int hours, int minutes, int seconds)
	: seconds(seconds), minutes(minutes), hours(hours), day(day) {
}

Time::Time(const time_t time)
{
	tm local;
	localtime_s(&local, &time);

	seconds = local.tm_sec;
	minutes = local.tm_min;
	hours = local.tm_hour;
	day = local.tm_wday;
}

Time Time::now()
{
	return Time(time(nullptr));
}

void Time::addTime(const Time& time)
{
	hours += time.hours;
	minutes += time.minutes;
	seconds += time.seconds;

	while (seconds >= 60) {
		minutes++;
		seconds -= 60;
	}
	while (seconds < 0) {
		minutes--;
		seconds += 60;
	}

	while (minutes >= 60)
	{
		hours++;
		minutes -= 60;
	}
	while (minutes < 0) {
		hours--;
		minutes += 60;
	}

	while (hours >= 24) {
		day++;
		hours -= 24;
	}
	while (hours < 0) {
		day--;
		hours += 24;
	}

	while (day > Saturday)
		day -= 6;
	while (day < Sunday)
		day += 6;
}

std::string Time::toString(const Time& time)
{
	std::stringstream ss;
	ss << time;

	return ss.str();
}

std::string Time::weekDayToString(int day)
{
	switch (day)
	{
	case Sunday:
		return "Sunday";
	case Monday:
		return "Monday";
	case Tuesday:
		return "Tuesday";
	case Wednesday:
		return "Wednesday";
	case Thursday:
		return "Thursday";
	case Friday:
		return "Friday";
	case Saturday:
		return "Saturday";
	default:
		return std::string();
	}
}

bool Time::isEqual(const Time& time) const
{
	if (seconds == time.seconds	&& minutes == time.minutes
		&& hours == time.hours && day == time.day)
		return true;
	return false;
}

bool Time::isSmaller(const Time& time) const
{
	if (day < time.day)
		return true;
	else if (day > time.day)
		return false;

	if (hours < time.hours)
		return true;
	else if (hours > time.hours)
		return false;

	if (minutes < time.minutes)
		return true;
	else if (minutes > time.minutes)
		return false;

	if (seconds < time.seconds)
		return true;
	else if (seconds > time.seconds)
		return false;

	return false;
}

bool Time::isBigger(const Time& time) const
{
	if (!isEqual(time) && !isSmaller(time))
		return true;

	return false;
}

bool Time::operator==(const Time& t2) const
{
	return isEqual(t2);
}

bool Time::operator<(const Time& t2) const
{
	return isSmaller(t2);
}

bool Time::operator>(const Time& t2) const
{
	return isBigger(t2);
}

bool Time::operator<=(const Time& t2) const
{
	return (isSmaller(t2) || isEqual(t2));
}

bool Time::operator>=(const Time& t2) const
{
	return (isBigger(t2) || isEqual(t2));
}

std::ostream& operator<<(std::ostream& os, const Time& time)
{
	os << std::right << std::setfill('0');
	os << std::setw(2) << time.hours;
	os << ":" << std::setw(2) << time.minutes;
	os << ":" << std::setw(2) << time.seconds << "  ";
	os << std::setfill(' ');
	os << std::setw(11) << std::left << Time::weekDayToString(time.day);

	return os;
}