#include <iostream>
#include <Windows.h>

#include "Log.h"

namespace Mineclone {

	HANDLE Log::s_console = GetStdHandle(STD_OUTPUT_HANDLE);

	Log::Log(const LogLevel& logLevel, std::string name) 
		: m_logLevel(logLevel), m_name(name) 
	{

	}

	Log::Log(std::string name)
#ifdef _DEBUG // Running in debug mode
		: m_logLevel(LogLevel::Debug), // If in debug mode, set log level to Debug
#else
		: m_logLevel(LogLevel::Info), // Otherwise set it to Info
#endif
		m_name(name) // Initialize the name variable as normal
	{

	}

	Log::Log()
		: m_logLevel(LogLevel::Info) 
	{

	}

	Log::~Log() {

	}

	void Log::setConsoleColor(Color color) {
		SetConsoleTextAttribute(s_console, (int) color);
	}

	void Log::resetConsoleColor() {
		SetConsoleTextAttribute(s_console, (int) Color::LightGrey);

	}
	void Log::printColored(const std::string& message, const Color& color)
	{
		setConsoleColor(color);
		std::cout << message << "\n";
		resetConsoleColor();
	}
}