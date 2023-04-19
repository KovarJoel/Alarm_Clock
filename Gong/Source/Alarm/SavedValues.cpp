// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "SavedValues.h"
#include <filesystem>
#include <fstream>
#include <array>

#define TO_TEXT(x) #x

void SavedValues::createFile()
{
	(void)savedPath();
}

bool SavedValues::deleteValueByID(const std::string& ID)
{
	std::ifstream fileIn(savedPath());
	std::ofstream fileOut(savedPath() + "x");
	if (!fileIn.good() || !fileOut.good())
		return false;

	bool found = false;
	std::string line;
	while (std::getline(fileIn, line))
	{
		std::string item;
		std::stringstream(line) >> item;

		if (item == ID)
			found = true;
		else
			fileOut << line << std::endl;
	}

	fileIn.close();
	fileOut.close();

	std::filesystem::remove(savedPath());
	std::filesystem::rename(savedPath() + "x", savedPath());
	return found;
}

void SavedValues::saveAll(const SavedValues& values)
{
	saveAlarms(values.alarms);
	saveStyleColor(values.styleColor);
	saveVolume(values.volume);
	saveCurrentFile(values.currentFile);
}

template <typename T>
static bool SavedValues::saveValueByID(const std::string& ID, const T& value)
{
	std::ifstream fileIn(savedPath());
	std::ofstream fileOut(savedPath() + "x");
	if (!fileIn.good() || !fileOut.good())
		return false;
	bool found = false;
	std::string line;
	while (std::getline(fileIn, line))
	{
		std::string item;
		std::stringstream(line) >> item;

		if (item == ID)
		{
			found = true;
			fileOut << ID << " = " << value << std::endl;
		}
		else
			fileOut << line << std::endl;
	}

	if (!found)
	{
		fileOut.clear();
		fileOut << ID << " = " << value << std::endl;
	}

	fileIn.close();
	fileOut.close();

	std::filesystem::remove(savedPath());
	std::filesystem::rename(savedPath() + "x", savedPath());
	return true;
}

void SavedValues::saveAlarms(const std::vector<Alarm>& alarms)
{
	std::array<std::string, 5> members;
	members = { "time.day", "time.hours", "time.minutes", "time.seconds", "enabled" };

	int i = alarms.size(), j = 0;
	// delete old values
	while (deleteValueByID(std::string(TO_TEXT(Alarms)) + "[" + std::to_string(i) + "]." + members[j]))
	{
		j++;
		if (j >= 5)
		{
			j = 0;
			i++;
		}
	}

	for (i = 0; i < alarms.size(); i++)
		saveAlarm(TO_TEXT(Alarms), i, alarms[i]);
}

void SavedValues::saveAlarm(const std::string& ID, size_t index, const Alarm& alarm)
{
	std::array<std::string, 5> members;
	members = { "time.day", "time.hours", "time.minutes", "time.seconds", "enabled" };

	std::string item;
	std::stringstream ss;
	ss << ID << "[" << index << "].";
	item = ss.str();

	int values[5]{};
	memcpy_s(values, 5 * sizeof(int), &alarm.time.day, 5 * sizeof(int));

	for (int i = 0; i < members.size(); i++)
		saveValueByID(item + members[i], values[i]);
}

void SavedValues::saveStyleColor(unsigned int styleColor)
{
	saveValueByID(TO_TEXT(StyleColor), styleColor);
}

void SavedValues::saveVolume(float volume)
{
	saveValueByID(TO_TEXT(Volume), volume);
}

void SavedValues::saveCurrentFile(unsigned int currentFile)
{
	saveValueByID(TO_TEXT(CurrentFile), currentFile);
}

const SavedValues& SavedValues::loadAll()
{
	static SavedValues values;
	
	values.alarms = loadAlarms();
	values.styleColor = loadStyleColor();
	values.volume = loadVolume();
	values.currentFile = loadCurrentFile();

	return values;
}

template <typename T>
T SavedValues::getValueByID(const std::string& ID, bool* outFound)
{
	std::ifstream file(savedPath());
	if (outFound)
		*outFound = false;
	if (!file.good())
		return T();

	T value{};
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream stream(line);
		std::string item;

		stream >> item;
		if (item.substr(0, 2) == "//")
			continue;
		if (item != ID)
			continue;

		char chEquals;
		stream >> chEquals >> value;

		if (outFound)
			*outFound = true;
		break;
	}

	file.close();
	return value;
}

std::vector<Alarm> SavedValues::loadAlarms()
{
	std::vector<Alarm> alarms;

	int i = 0;
	bool found = false;

	while (true)
	{
		Alarm a = loadAlarm(TO_TEXT(Alarms), i++, &found);
		if (found)
			alarms.emplace_back(a);
		else
			break;
	}

	return alarms;
}

Alarm SavedValues::loadAlarm(const std::string& ID, size_t index, bool* outFound)
{
	std::string item;
	Alarm alarm;
	int value[5]{};
	std::stringstream ss;
	ss << ID << "[" << index << "].";
	item = ss.str();

	std::array<std::string, 5> members;
	members = { "time.day", "time.hours", "time.minutes", "time.seconds", "enabled" };
	
	for (int i = 0; i < members.size(); i++)
		value[i] = getValueByID<int>(item + members[i], outFound);			
	
	if (outFound)
		if (*outFound)
			memcpy_s(&alarm.time.day, 5 * sizeof(int), value, 5 * sizeof(int));

	return alarm;
}

unsigned int SavedValues::loadStyleColor()
{
	return getValueByID<unsigned int>(TO_TEXT(StyleColor));
}

float SavedValues::loadVolume()
{
	bool found = false;
	float volume = getValueByID<float>(TO_TEXT(Volume), &found);

	return (found ? volume : 100.0f);
}

unsigned int SavedValues::loadCurrentFile()
{
	return getValueByID<unsigned int>(TO_TEXT(CurrentFile));
}

const std::string& SavedValues::savedPath()
{
	static std::string path;
	{
		char* buffer;
		size_t size;

		_dupenv_s(&buffer, &size, "AppData");
		if (!buffer)
		{
			path.clear();
			return path;
		}
		path = buffer;

		path += "\\Gong\\Data\\";
		if (!std::filesystem::exists(path))
			std::filesystem::create_directory(path);

		path += "saved.txt";
	}

	if (!std::filesystem::exists(path))
	{
		std::ofstream file(path);
		file.close();
	}
	
	return path;
}