#pragma once
#include <iostream>
#include <vector>
#include "Alarm.h"

class SavedValues
{
private:
	std::vector<Alarm> alarms;
	unsigned int styleColor;
	unsigned int currentFile;
	float volume;

	enum ItemIDs {
		Alarms, StyleColor, Volume, CurrentFile
	};

	SavedValues() = default;
public:
	static void createFile();

	static const SavedValues& loadAll();
	static std::vector<Alarm> loadAlarms();
	static unsigned int loadStyleColor();
	static float loadVolume();
	static unsigned int loadCurrentFile();

	static void saveAll(const SavedValues& values);
	static void saveAlarms(const std::vector<Alarm>& alarms);
	static void saveStyleColor(unsigned int styleColor);
	static void saveVolume(float volume);
	static void saveCurrentFile(unsigned int currentFile);

private:
	static const std::string& savedPath();

	template <typename T>
	static T getValueByID(const std::string& ID, bool* outFound = nullptr);
	template <typename T>
	static bool saveValueByID(const std::string& ID, const T& value);
	static bool deleteValueByID(const std::string& ID);
	static Alarm loadAlarm(const std::string& ID, size_t index, bool* outFound = nullptr);
	static void saveAlarm(const std::string& ID, size_t index, const Alarm& alarm);
};