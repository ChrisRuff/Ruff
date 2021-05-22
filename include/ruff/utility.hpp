#pragma once
#include <vector>
#include <optional>
#include <filesystem>
#include <fstream>
#include <concepts>

namespace ruff
{
namespace fio
{
	template<typename T> requires std::convertible_to<T, std::string>
	bool write( const std::vector<T>& data, 
			const std::filesystem::path file, const char spacer = ',')
	{
		std::ofstream out_file{file.string()};
		if(!out_file.is_open())
		{
			return false;
		}
		for(auto it = data.begin(); it != data.end(); ++it)
		{
			out_file << static_cast<std::string>(*it) << spacer;
		}
		out_file.close();
		return true;
	}
	template<typename T>
	[[nodiscard]] std::optional<std::vector<T>> 
		read(const std::filesystem::path file, const char delimeter = ',')
	{
		std::vector<T> data;
		
		std::ifstream in_file{file.string()};
		if(!in_file.is_open())
		{
			return std::optional<std::vector<T>>();
		}
		std::string line{};
		while(getline(in_file, line, delimeter))
		{
			data.emplace_back(line);
		}
		return data;
	}
}
}
