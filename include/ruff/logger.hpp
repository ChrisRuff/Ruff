#pragma once
#include <concepts> // is_convertible_to
#include <type_traits>
#include <iostream>
#include <string>

namespace ruff
{
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
		friend std::ostream& operator<<(std::ostream& os, const Modifier& mod)
		{
			return os << "\033[" << static_cast<int>(mod.code) << "m";
		}
	};
}// namespace color

static color::Modifier yellow(color::FG_LIGHT_YELLOW);
static color::Modifier red(color::FG_RED);
static color::Modifier defaultFG(color::FG_DEFAULT);

inline void printHeader(const std::string_view title)
{
	std::cout << "~~~~~~~~~~~~ " << title << " ~~~~~~~~~~~~\n";
}
inline void printHeader()
{
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

template<typename T> requires std::convertible_to<T, const std::string>
inline void logInfo(const T& message)
{
	printHeader("INFO");
	std::cout << message << std::endl;
	printHeader();
};

template<typename T> requires std::convertible_to<T, const std::string>
inline void logWarning(const T& message)
{
	std::cout << yellow << std::endl;
	printHeader("WARN");
	std::cout << message;
	std::cout << defaultFG << std::endl;
};

template<typename T> requires std::convertible_to<T, const std::string>
inline void logError(const T& message)
{
	std::cout << red << std::endl;
	printHeader("ERRR");
	std::cout << message;
	std::cout << defaultFG << std::endl;
};
}// namespace ruff
