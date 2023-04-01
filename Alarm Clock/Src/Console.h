#pragma once
#include <Windows.h>
#include <iostream>

enum Color
{
	Black, Navy, Green, Teal, Maroon, Purple, Olive, Silver,
	Grey, Blue, Lime, Aqua, Red, Fuchisa, Yellow, White,
	CURRENT
};

class Console
{
public:
	static void setTitle(const std::string& title);
	static void disableHighlighting(bool disabled = true);
	static void setCursorVisibility(bool visible);

	static WORD getColor();
	static void setColor(WORD color);
	static void setColor(Color background, Color foreground);

	static COORD getFontSize();

	static COORD getCursorPosition();
	static void setCursorPosition(COORD coord);
	static void setCursorPosition(short x, short y);

	static POINT getMousePoint();
	static COORD getMousePosition();
};