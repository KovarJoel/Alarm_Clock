#include "AlarmClock.h"

AlarmClock::AlarmClock()
	:size(500, 500), position(200, 100) {
}

AlarmClock::~AlarmClock()
{
	ring.join();
}

void AlarmClock::init(GLFWwindow* window)
{
	if (isRunning)
		return;

	this->window = window;
	
	sound.setPath("Dependencies\\gong.wav");

	alarms.push_back(Alarm(Time(Saturday, 10, 25, 0)));
	alarms.push_back(Alarm(Time(Saturday, 10, 30, 0)));
	sortAlarms();
	
	isRunning = true;
	ring = std::thread(&AlarmClock::ringCallback, this);
}

void AlarmClock::handleEvents()
{

}

void AlarmClock::update()
{
	//sortAlarms();
	//sortAlarmsUpcoming();

	glfwSetWindowSize((GLFWwindow*)window, size.x, size.y);
	glfwSetWindowPos((GLFWwindow*)window, position.x, position.y);

	if (!isRunning)
		glfwSetWindowShouldClose((GLFWwindow*)window, GLFW_TRUE);
}

void AlarmClock::render()
{
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoResize;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
	//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize;
	
	ImGui::Begin("Alarm Clock", &isRunning, flags);
	static bool doOnce = true;
	if (doOnce) {
		ImGui::SetWindowSize(size);
		ImGui::SetWindowPos(position);
		doOnce = false;
	}
	
	ImGui::SameLine();
	ImGui::Text(Time::toString(Time::now()).c_str());
	ImGui::NewLine();

	ImGui::SeparatorText("Upcoming Alarms");

	static int child = -1;
	for (int i = 0; i < alarms.size(); i++)
	{
		std::string hiddenID = "##" + std::to_string(i);
		ImGui::Text(Time::toString(alarms.at(i).time).c_str());

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		ImGui::SliderInt((hiddenID + " toggle ").c_str(), &alarms.at(i).enabled, 0, 1, (alarms.at(i).enabled ? "ON" : "OFF"));

		ImGui::SameLine();
		if (ImGui::Button(("EDIT" + hiddenID).c_str()))
			child = i;

		ImGui::SameLine();
		if (ImGui::Button(("DELETE" + hiddenID).c_str()))
		{
			alarms.erase(alarms.begin() + i);

			if (child == i)
				child = -1;
			if (child > i)
				child--;
		}

		if (child == i)
		{
			ImGui::BeginChild(ImGui::GetID("Alarm Clock"), ImVec2(0, ImGui::GetFontSize() * 4.0f), false, flags);
			float width = 100.0f;
			ImGui::SetNextItemWidth(width);
			ImGui::SliderInt("##edit hours", &alarms.at(child).time.hours, 0, 23);
			ImGui::SameLine(0.0f, 4.0f);
			ImGui::SetNextItemWidth(width * 2.0f + 4.0f);
			ImGui::SliderInt2("##edit rest", &alarms.at(child).time.minutes, 0, 59);
			ImGui::SameLine(0.0f, 4.0f);
			ImGui::SetNextItemWidth(width);
			ImGui::SliderInt("##edit day", (int*)&alarms.at(child).time.day, 0, 6, Time::weekDayToString(alarms.at(child).time.day).c_str());
			if (ImGui::Button("Done"))
			{
				sortAlarms();
				child = -1;
			}
			ImGui::EndChild();
		}
	}

	ImGui::NewLine();
	ImGui::NewLine();
	if (ImGui::Button("Add Alarm"))
	{
		Time last = Time::now();
		last.addTime(Time(Sunday, 0, 0, -1));
		alarms.push_back(Alarm(last));

		child = alarms.size() - 1;
	}
	ImGui::NewLine();
	
	size = ImGui::GetWindowSize();
	position = ImGui::GetWindowPos();

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