#pragma once

#include <Windows.h>

// Perhaps this shouldn't be a class, and only contain non-member functions.
// https://stackoverflow.com/a/112451/7975284 Reference for the above

namespace Mineclone {
	class Log {
	public:

		enum class LogLevel {
			Error, // 0: Log errors only
			Warning, // 1: Log only warnings and errors
			Info, // 2: Log normal info messages, warnings and errors
			Debug // 3: Log everything
		};

		Log(const LogLevel& logLevel, std::string name);
		Log(std::string name); // Default log level is INFO, in debug mode it's DEBUG
		Log();
		~Log();

		void setLogLevel(const LogLevel& level) { m_logLevel = level; }

		template <typename T>
		void error(const T& message) {
			if(m_logLevel >= LogLevel::Error)
				printColored("[ERROR] " + m_name + ": " + message, Color::Red);
		}

		template <typename T>
		void warn(const T& message) {
			if(m_logLevel >= LogLevel::Warning) 
				printColored("[WARN] " + m_name + ": " + message, Color::Yellow);
		}

		template <typename T>
		void info(const T& message) {
			if(m_logLevel >= LogLevel::Info)
				printColored("[INFO] " + m_name + ": " + message, Color::LightGrey);
		}

		template <typename T>
		void debug(const T& message) {
			if(m_logLevel >= LogLevel::Debug)
				printColored("[DEBUG] " + m_name + ": " + message, Color::DarkGreyg);
		}

	private:
		enum class Color {
			DarkBlue = 1,
			DarkGreen,
			DarkCyan,
			DarkRed,
			DarkPurple,
			DarkYellow,
			LightGrey,
			DarkGrey,
			Blue,
			Green,
			Cyan,
			Red,
			Pink,
			Yellow,
			White
		};

		static void setConsoleColor(Color color);
		static void resetConsoleColor();

		void printColored(const std::string& message, const Color& color);

		static HANDLE s_console;

		LogLevel m_logLevel = LogLevel::Info;
		const std::string m_name;

	};
}