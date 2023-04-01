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
	flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground;
	flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::GetStyle().Colors[ImGuiCol_Button].w = 0.0f;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
	//ImGui::GetStyle().ButtonTextAlign = ImVec2(0.8f, 0.5f);


	ImGui::Begin(" ", nullptr, flags);
	ImGui::SameLine();
	ImGui::Text(Time::toString(Time::now()).c_str());
	ImGui::NewLine();

	ImGui::SeparatorText("Upcoming Alarms");
	
	static int child = -1;
	for (size_t i = 0; i < alarms.size(); i++)
	{
		if (ImGui::Button(Time::toString(alarms.at(i).time).c_str()))
			child = i;

		/*
			ImGui::SliderInt3(("Alarm " + std::to_string(i)).c_str(), &alarms.at(i).time.hours, 0, 59);
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);
			//<mGui::DragScalar("D", ImGuiDataType_::ImGuiDataType_S8, &alarms.at(i).time.day,
				//0.04F, (const void*)0, (const void*)0, Time::weekDayToString(alarms.at(i).time.day).c_str());
			ImGui::PopItemWidth();
			if (alarms.at(i).time.day > Saturday)
				alarms.at(i).time.day = Sunday;
			if (alarms.at(i).time.day < Sunday)
				alarms.at(i).time.day = Saturday;
			ImGui::SameLine();
			//ImGui::DragScalar(("Enabled " + std::to_string(i)).c_str(), ImGuiDataType_::ImGuiDataType_U8);
			//ImGui::Checkbox(("Enabled " + std::to_string(i)).c_str(), & alarms.at(i).enabled);
			//ImGui::InvisibleButton("Button Test", (ImVec2)())
		*/
	}

	if (child != -1)
	{
		ImGuiWindowFlags childFlags = 0;
		childFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground;
		childFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
		childFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
		childFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::BeginChild(ImGui::GetID("Upcoming Alarms"), ImVec2(400, 100), false, childFlags);

		ImGui::SliderInt3("Time", &alarms.at(child).time.hours, 0, 59);
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