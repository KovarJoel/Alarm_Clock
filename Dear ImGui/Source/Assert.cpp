#include "Assert.h"

void Assert::assertCodeMsg(const std::string& file, const int line, const std::string& function, const std::string& message)
{
	std::cout << "ASSERT  >>   * FILE:       " << file << "\n";
	std::cout << "             * FUNCTION:   " << function << "\n";
	std::cout << "             * LINE:       " << line << "\n";
	std::cout << "             * MESSAGE:    " << message << std::endl;
}

void Assert::assertMsg(const std::string& message)
{
	std::cout << "ASSERT  >>   * " << message << "\n";
}

void Assert::assertGlfwCallback(int error, const char* description)
{
	std::cout << "ASSERT  >>   * GLFW ERROR CALLBACK\n";
	std::cout << "             * ERROR:      " << error << "\n";
	std::cout << "             * MESSAGE:    " << description << std::endl;
}