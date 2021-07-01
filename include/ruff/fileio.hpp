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
	template<typename T, typename TP=T::value_type> requires std::convertible_to<TP, std::string>
	bool write( const T data_start, const T data_end,
			const std::filesystem::path file, const char spacer = ',')
	{
		std::ofstream out_file{file.string()};
		if(!out_file.is_open())
		{
			return false;
		}
		for(auto it = data_start; it != data_end; ++it)
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
