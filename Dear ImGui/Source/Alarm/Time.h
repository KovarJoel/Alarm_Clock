#pragma once
#include <time.h>
#include <iostream>
#include <iomanip>
#include <sstream>

enum WeekDay : signed char
{
	Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday
};

class Time
{
public:
	int hours;
	int minutes;
	int seconds;
	WeekDay day;

	Time();
	Time(WeekDay day, int hours, int minutes, int seconds);
	Time(const time_t time);
	~Time() = default;

	static Time now();

	static std::string toString(const Time& time);
	static std::string weekDayToString(WeekDay day);

	bool isEqual(const Time& time);
	bool isSmaller(const Time& time);
	bool isBigger(const Time& time);

	bool operator==(const Time& t2);
	bool operator<(const Time& t2);
	bool operator>(const Time& t2);
	bool operator<=(const Time& t2);
	bool operator>=(const Time& t2);

	friend std::ostream& operator<<(std::ostream& os, const Time& time);
};