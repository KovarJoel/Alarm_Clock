#pragma once
#include <time.h>
#include <iostream>
#include <iomanip>
#include <sstream>

enum WeekDay
{
	Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday
};

class Time
{
public:
	int day, hours, minutes, seconds;

	Time();
	Time(int day, int hours, int minutes, int seconds);
	Time(const time_t time);
	~Time() = default;

	static Time now();
	static Time last();

	void addTime(const Time& time);

	static std::string toString(const Time& time);
	static std::string weekDayToString(int day);

	bool isEqual(const Time& time) const;
	bool isSmaller(const Time& time) const;
	bool isBigger(const Time& time) const;

	bool operator==(const Time& t2) const;
	bool operator<(const Time& t2) const;
	bool operator>(const Time& t2) const;
	bool operator<=(const Time& t2) const;
	bool operator>=(const Time& t2) const;

	Time operator+(const Time& t2) const;
	Time operator-(const Time& t2) const;

	friend std::ostream& operator<<(std::ostream& os, const Time& time);
};