#pragma once
#pragma warning( disable : 4369)	// enumerator value cannot be represented as char
#include <chrono>
#include "Console.h"

class Button
{
private:
	std::string text;
	
	COORD start, end;
	WORD color;

	enum FrameSigns : char {
		TopLeft = 218, TopRight = 191, BottomLeft = 192, BottomRight = 217, Horizontal = 196, Vertical = 179
	};

public:

	enum TextLocation {
		Left, Right, Middle, Top, Bottom
	};

	bool hasFrame;

	Button(const std::string& text, COORD start);
	Button(const std::string& text, COORD start, COORD end);
	~Button() = default;

	void setText(const std::string& text);
	void setPosition(COORD start);
	void setPosition(COORD start, COORD end);
	void setColor(WORD color);
	void setColor(Color background, Color foreground);

	bool isPressed();

	void print();
	void print(TextLocation horizontal, TextLocation vertical);

	//friend std::ostream& operator<<(std::ostream& os, const Button& button);
};