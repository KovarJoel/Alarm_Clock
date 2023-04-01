#include "Application.h"

Application::Application(const std::string& windowTitle)
	: isRunning(true)
{
	setupConsole(windowTitle);

	ring = std::thread(&Application::ringCallback, this);
	sound.setPath("C:\\Users\\abc\\Desktop\\gong.mp3");

	for (int i = 0; i < 10; i++)
		alarms.push_back(Alarm(Time(Saturday, 18, 11, 2)));
}

Application::~Application()
{
	ring.join();
}

void Application::handleEvents()
{
	char key = 0;

	if (_kbhit())
		key = _getch();

	if (key == 27)	// ESC
		isRunning = false;

	for (auto& a : alarms)
	{
		if (a.button->isPressed())
			a.toggle();
	}
}

void Application::render()
{
	using namespace std::chrono_literals;

	Time now = Time::now();
	static Time last;

	Console::setCursorPosition(0, 0);

	// header
	std::cout << "\n";
	std::cout << " ******************************\n";
	std::cout << "      " << now << "            \n";
	std::cout << " ******************************\n";
	std::cout << "\n\n";

	// alarms
	if (alarms.size())
	{
		sortAlarms();

		std::cout << " Upcoming alarms:\n";
		for (size_t i = 0; i < alarms.size(); i++)
		{
			COORD pos{};

			if (i >= 10)
			{
				std::cout << "\n ... and " << (alarms.size() - i) << " more.";
				break;
			}
			std::cout << "\n " << std::setw(2) << (i + 1) << ". " << alarms.at(i).time << "  |  ";

			alarms.at(i).button->setPosition(Console::getCursorPosition());
			alarms.at(i).button->setText((alarms.at(i).enabled ? " ON" : "OFF"));
			alarms.at(i).button->setColor(static_cast<Color>((Console::getColor() & 0xF0) >> 4), (alarms.at(i).enabled ? Green : Red));
			alarms.at(i).button->print();
		}

		std::cout << "\n\n";
	}

	last = now;
	//std::this_thread::sleep_for(100ms);
}

void Application::sortAlarms()
{
	Alarm temp;
	bool changes = false;
	
	do
	{
		changes = false;

		for (size_t i = 0; i < alarms.size() - 1; i++) {
			if (alarms.at(i).time > alarms.at(i + 1).time) {
				temp = alarms.at(i);
				alarms.at(i) = alarms.at(i + 1);
				alarms.at(i + 1) = temp;

				changes = true;
			}
		}
	} while (changes);
	
	// sort for next
	std::vector<Alarm> temps = alarms;
	Time now = Time::now();
	size_t next = 0;

	for (size_t i = 0; i < alarms.size(); i++) {
		if (alarms.at(i).time >= now){
			next = i;
			break;
		}
	}

	for (size_t i = next, j = 0; j < alarms.size(); i++, j++)
	{
		if (i < alarms.size())
			alarms.at(j) = temps.at(i);
		else
			alarms.at(j) = temps.at(i - alarms.size());
	}
}

void Application::setupConsole(const std::string& windowTitle)
{
	system("color f0");

	Console::setTitle(windowTitle);
	Console::disableHighlighting();
	Console::setCursorVisibility(false);
	Console::setColor(White, Black);
}

void Application::ringCallback()
{
	using namespace std::chrono_literals;
	
	Time now = Time::now();
	Time lastRing;
	
	while (isRunning)
	{
		now = Time::now();

		if (now == lastRing)
			continue;

		for (auto& alarm : alarms)
		{
			if (!alarm.enabled)
				continue;
			if (!(now == alarm.time))
				continue;

			sound.play();
			lastRing = now;
		}

		std::this_thread::sleep_for(200ms);
	}
}