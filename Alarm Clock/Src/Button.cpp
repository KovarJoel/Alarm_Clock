#include "Button.h"

Button::Button(const std::string& text, COORD start)
	: text(text)
{
	hasFrame = false;
	
	setPosition(start);

	color = Console::getColor();
}

Button::Button(const std::string& text, COORD start, COORD end)
	: text(text), start(start)
{
	hasFrame = false;

	setPosition(start, end);

	color = Console::getColor();
}

void Button::setText(const std::string& text)
{
	this->text = text;
}

void Button::setPosition(COORD start, COORD end)
{
	this->start = start;

	if (end.X < start.X + static_cast<short>(text.size()) + 1)
		this->end.X = start.X + static_cast<short>(text.size()) + 1;
	else
		this->end = end;

	if (end.Y < start.Y + 2)
		this->end.Y = start.Y + 2;
	else
		this->end.Y = end.Y;
}

void Button::setPosition(COORD start)
{
	this->start = start;

	end.X = start.X + static_cast<short>(text.size()) + (hasFrame ? 1 : 0);
	end.Y = start.Y + (hasFrame ? 2 : 0);
}

void Button::setColor(WORD color)
{
	this->color = color;
}

void Button::setColor(Color background, Color foreground)
{
	color = 0;
	color |= (background << 4) & 0xF0;
	color |= foreground & 0x0F;
}

bool Button::isPressed()
{
	using namespace std::chrono;

	COORD coord = Console::getMousePosition();
	long long now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	static long long lastHit = 0;

	if (now - 250 < lastHit)
		return false;

	if (!(GetKeyState(VK_LBUTTON) & 0x8000))
		return false;
	if (coord.X < start.X || coord.X > end.X)
		return false;
	if (coord.Y < start.Y || coord.Y > end.Y)
		return false;

	lastHit = now;
	return true;
}

void Button::print(TextLocation horizontal, TextLocation vertical)
{
	WORD windowColor = Console::getColor();
	Console::setColor(color);

	if (hasFrame)
	{
		Console::setCursorPosition(start);
		std::cout << TopLeft;
		Console::setCursorPosition(end.X, start.Y);
		std::cout << TopRight;
		Console::setCursorPosition(end);
		std::cout << BottomRight;
		Console::setCursorPosition(start.X, end.Y);
		std::cout << BottomLeft;

		for (short i = start.X + 1; i < end.X; i++)
		{
			Console::setCursorPosition(i, start.Y);
			std::cout << Horizontal;
			Console::setCursorPosition(i, end.Y);
			std::cout << Horizontal;
		}

		for (short i = start.Y + 1; i < end.Y; i++)
		{
			Console::setCursorPosition(start.X, i);
			std::cout << Vertical;
			Console::setCursorPosition(end.X, i);
			std::cout << Vertical;
		}
	}

	COORD textStart;

	switch (horizontal)
	{
	case Button::Left:
		textStart.X = start.X + (hasFrame ? 1 : 0);
		break;
	case Button::Right:
		textStart.X = end.X - static_cast<short>(text.size());
		break;
	case Button::Middle:
	default:
		textStart.X = start.X + (end.X - start.X) / 2;
		textStart.X -= static_cast<short>(text.size()) / 2;
		if (!(text.size() % 2))
			textStart.X += 1;
		break;
	}

	switch (vertical)
	{
	case Button::Top:
		textStart.Y = start.Y + (hasFrame ? 1 : 0);
		break;
	case Button::Bottom:
		textStart.Y = end.Y - (hasFrame ? 1 : 0);
		break;
	case Button::Middle:
	default:
		textStart.Y = start.Y + (end.Y - start.Y) / 2;
		break;
	}

	Console::setCursorPosition(textStart);
	std::cout << text;

	Console::setColor(windowColor);
}

void Button::print()
{
	print(Middle, Middle);
}