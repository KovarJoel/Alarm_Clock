#include "Console.h"

void Console::setTitle(const std::string& title)
{
	SetConsoleTitle(std::wstring(title.begin(), title.end()).c_str());
}

void Console::disableHighlighting(bool disabled)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD prev_mode;
	GetConsoleMode(hInput, &prev_mode);

	SetConsoleMode(hInput, prev_mode | ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);

	if(disabled)
		SetConsoleMode(hInput, prev_mode ^ ENABLE_QUICK_EDIT_MODE ^ ENABLE_EXTENDED_FLAGS);
}

void Console::setCursorVisibility(bool visible)
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hOutput, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(hOutput, &cursorInfo);
}

WORD Console::getColor()
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hOutput, &info);

	return info.wAttributes;
}

void Console::setColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Console::setColor(Color background, Color foreground)
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hOutput, &info);

	WORD current = info.wAttributes;
	WORD color = 0;

	if (background != CURRENT)
		color |= (background << 4) & 0xF0;
	else
		color |= current & 0xF0;
	
	if (foreground != CURRENT)
		color |= foreground & 0x0F;
	else
		color |= current & 0x0F;

	SetConsoleTextAttribute(hOutput, color);
}

COORD Console::getCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO info{};
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	return info.dwCursorPosition;
}

void Console::setCursorPosition(COORD coord)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Console::setCursorPosition(short x, short y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;

	setCursorPosition(coord);
}

POINT Console::getMousePoint()
{
	POINT point{};
	GetCursorPos(&point);
	ScreenToClient(GetForegroundWindow(), &point);

	return point;
}

COORD Console::getMousePosition()
{
	POINT point = getMousePoint();
	COORD coord{};
	coord.X = static_cast<short>(point.x / 8);
	coord.Y = static_cast<short>(point.y / 16);

	return coord;
}