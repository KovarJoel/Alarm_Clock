#include "AlarmClock.h"

AlarmClock::AlarmClock()
{
	init();
}

AlarmClock::~AlarmClock()
{
	ring.join();
}

void AlarmClock::init()
{
	if (isRunning)
		return;

	ring = std::thread(&AlarmClock::ringCallback, this);
	sound.setPath("C:\\Users\\abc\\Desktop\\gong.mp3");

	alarms.push_back(Alarm(Time::now()));
	alarms.push_back(Alarm(Time(Saturday, 8, 0, 0)));
	alarms.push_back(Alarm(Time(Monday, 6, 0, 0)));
	alarms.push_back(Alarm(Time(Saturday, 12, 12, 12)));
	alarms.push_back(Alarm(Time(Wednesday, 20, 20, 20)));
	sortAlarms();
	
	isRunning = true;
}

void AlarmClock::handleEvents()
{

}

void AlarmClock::update()
{
	sortAlarms();
	//sortAlarmsUpcoming();
}

void AlarmClock::render()
{
	ImGuiWindowFlags flags = 0;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoResize;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize;

	//ImGui::GetStyle().Colors[ImGuiCol_Button].w = 0.0f;
	//ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
	//ImGui::GetStyle().ButtonTextAlign = ImVec2(0.8f, 0.5f);

	ImGui::Begin("Alarms", nullptr, flags);
	ImGui::SameLine();
	ImGui::Text(Time::toString(Time::now()).c_str());
	ImGui::NewLine();

	ImGui::SeparatorText("Upcoming Alarms");
	
	static int child = -1;
	for (size_t i = 0; i < alarms.size(); i++)
	{
		std::string hiddenID = "##" + std::to_string(i);

		ImGui::Text(Time::toString(alarms.at(i).time).c_str());

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		ImGui::SliderInt((hiddenID + " toggle ").c_str(), (int*)&alarms.at(i).enabled, 0, 1, (alarms.at(i).enabled ? "ON" : "OFF"));

		ImGui::SameLine();
		if (ImGui::Button(("EDIT" + hiddenID).c_str()))
			child = i;

		ImGui::SameLine();
		if (ImGui::Button(("DELETE" + hiddenID).c_str()))
		{
			alarms.erase(alarms.begin() + i);
			child = -1;
		}
	}

	if (child != -1)
	{
		ImGuiWindowFlags childFlags = 0;
		childFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground;
		childFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
		childFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
		childFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::BeginChild(ImGui::GetID("Upcoming Alarms"), ImVec2(0, 0), false, childFlags);

		//ImGui::SliderInt3("Time", &alarms.at(child).time.hours, 0, 59);
		float width = 100.0f;
		ImGui::SetNextItemWidth(width);
		ImGui::SliderInt("##edit hours", &alarms.at(child).time.hours, 0, 23);
		ImGui::SameLine(0.0f, 4.0f);
		ImGui::SetNextItemWidth(width * 2.0f + 4.0f);
		ImGui::SliderInt2("##edit rest", &alarms.at(child).time.minutes, 0, 59);

		if (ImGui::Button("Done"))
			child = -1;

		ImGui::EndChild();
	}

	ImGui::End();
}

void AlarmClock::close()
{
	isRunning = false;
}

void AlarmClock::sortAlarms()
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
}

void AlarmClock::sortAlarmsUpcoming()
{
	std::vector<Alarm> tempAlarms = alarms;
	Time now = Time::now();
	size_t next = 0;

	for (size_t i = 0; i < alarms.size(); i++) {
		if (alarms.at(i).time >= now) {
			next = i;
			break;
		}
	}

	for (size_t i = next, j = 0; j < alarms.size(); i++, j++)
	{
		if (i < alarms.size())
			alarms.at(j) = tempAlarms.at(i);
		else
			alarms.at(j) = tempAlarms.at(i - alarms.size());
	}
}

void AlarmClock::ringCallback()
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