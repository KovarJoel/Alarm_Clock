#include "Time.h"

Time::Time()
	: seconds(0), minutes(0), hours(0), day(Sunday) {
}

Time::Time(WeekDay day, int hours, int minutes, int seconds)
	: seconds(seconds), minutes(minutes), hours(hours), day(day) {
}

Time::Time(const time_t time)
{
	tm local;
	localtime_s(&local, &time);

	seconds = local.tm_sec;
	minutes = local.tm_min;
	hours = local.tm_hour;
	day = static_cast<WeekDay>(local.tm_wday);
}

Time Time::now()
{
	return Time(time(nullptr));
}

std::string Time::toString(const Time& time)
{
	std::stringstream ss;
	ss << time;

	return ss.str();
}

std::string Time::weekDayToString(WeekDay day)
{
	std::string dayName;

	switch (day)
	{
	case Sunday:
		dayName = "Sunday";
		break;
	case Monday:
		dayName = "Monday";
		break;
	case Tuesday:
		dayName = "Tuesday";
		break;
	case Wednesday:
		dayName = "Wednesday";
		break;
	case Thursday:
		dayName = "Thursday";
		break;
	case Friday:
		dayName = "Friday";
		break;
	case Saturday:
		dayName = "Saturday";
		break;
	default:
		break;
	}

	return dayName;
}

bool Time::isEqual(const Time& time)
{
	if (seconds == time.seconds	&& minutes == time.minutes
		&& hours == time.hours && day == time.day)
		return true;
	return false;
}

bool Time::isSmaller(const Time& time)
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

bool Time::isBigger(const Time& time)
{
	if (!isEqual(time) && !isSmaller(time))
		return true;

	return false;
}

bool Time::operator==(const Time& t2)
{
	return isEqual(t2);
}

bool Time::operator<(const Time& t2)
{
	return isSmaller(t2);
}

bool Time::operator>(const Time& t2)
{
	return isBigger(t2);
}

bool Time::operator<=(const Time& t2)
{
	return (isSmaller(t2) || isEqual(t2));
}

bool Time::operator>=(const Time& t2)
{
	return (isBigger(t2) || isEqual(t2));
}

std::ostream& operator<<(std::ostream& os, const Time& time)
{
	os << std::setfill('0');
	os << std::setw(2) << time.hours;
	os << ":" << std::setw(2) << time.minutes;
	os << ":" << std::setw(2) << time.seconds << "  ";
	os << std::setfill(' ');
	os << std::setw(11) << std::left << Time::weekDayToString(time.day);

	return os;
}