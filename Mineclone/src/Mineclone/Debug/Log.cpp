#include <iostream>
#include <Windows.h>
#include <string>

#include "Log.h"

namespace Mineclone {

	HANDLE Log::s_console = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string Log::s_lastMessage = std::string("");
	Mineclone::Log::Color Log::s_currentColor = Color::LightGrey;

	Log::Log(const LogLevel& logLevel, const std::string& name) 
		: m_logLevel(logLevel), m_name(std::move(name)) 
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
		: m_logLevel(LogLevel::Info), m_name("Logger")
	{

	}

	Log::~Log() {

	}

	void Log::setConsoleColor(const Color& color) {
		if(s_currentColor == color) return;
		SetConsoleTextAttribute(s_console, (int) color);
		s_currentColor = color;
	}

	void Log::resetConsoleColor() {
		setConsoleColor(Color::LightGrey);

	}
	void Log::printColored(const std::string& message, const Color& color)
	{
		if(message == s_lastMessage) return;

		setConsoleColor(color);
		std::cout << message << "\n";
		resetConsoleColor();

		s_lastMessage = message;
	}
}