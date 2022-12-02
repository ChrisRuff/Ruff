#pragma once
#include <iostream>
#include <string>

namespace ruff
{
enum class rcode
{
	FAILURE = 0,
	SUCCESS = 1
};

// Code from this stackover flow answer
// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
namespace color
{
	enum Code
	{
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_DEFAULT = 39,
		BG_RED = 41,
		BG_GREEN = 42,
		BG_BLUE = 44,
		BG_DEFAULT = 49,
		FG_LIGHT_YELLOW = 93
	};
	class Modifier
	{
	private:
		Code code;

	public:
		explicit Modifier(Code pCode) noexcept : code(pCode) {}
		friend std::ostream& operator<<(std::ostream& os,
		                                const Modifier& mod)
		{
			return os << "\033[" << static_cast<int>(mod.code) << "m";
		}
	};
}// namespace color

static color::Modifier blue(color::FG_BLUE);
static color::Modifier yellow(color::FG_LIGHT_YELLOW);
static color::Modifier red(color::FG_RED);
static color::Modifier defaultFG(color::FG_DEFAULT);

inline void log(const std::string& message)
{
#ifndef NDEBUG
	std::cout << blue << "\n## INFO: " << message << " ##" << defaultFG
	          << std::endl;
#else
	(void)message;
#endif
};
inline void logWarning(const std::string& message)
{
	std::cout << yellow;
	std::cout << "\n## WARNING: " << message << " ##" << std::endl;
	std::cout << defaultFG;
};
inline void logError(const std::string& message)
{
	std::cout << red;
	std::cout << "\n## ERROR: " << message << " ##" << std::endl;
	std::cout << defaultFG;
};
inline void logInfo(const std::string& message)
{
	std::cout << "\n## INFO: " << message << " ##" << std::endl;
	std::cout << defaultFG;
};
}// namespace ruff
