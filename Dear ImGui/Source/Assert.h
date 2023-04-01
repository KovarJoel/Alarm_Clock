#pragma once
#include <iostream>

#define ASSERT_MSG(message) Assert::assertCodeMsg(__FILE__, __LINE__, __FUNCTION__, message)

namespace Assert
{
	void assertCodeMsg(const std::string& file, const int line, const std::string& function, const std::string& message);
	void assertMsg(const std::string& message);
	void assertGlfwCallback(int error, const char* description);
}