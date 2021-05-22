#include "doctest/doctest.h"

#include <iostream>
#include <cmath>

#include "utility.hpp"

struct Integer
{
	int val;
	Integer(int val) : val(val) {}
	Integer(std::string val) : val(std::stoi(val)) {}
	operator std::string() const
	{
		return std::to_string(val);
	}
	bool operator==(const Integer& other) const { return val == other.val; }
};

TEST_CASE("Read-Write to file")
{
	std::vector<Integer> vals{1,2,3,4,5,6,7,8,9,10};

	// Requires convertable to string template
	ruff::fio::write<Integer>(vals, "/tmp/ruff");

	std::optional<std::vector<Integer>> vals_in = 
		ruff::fio::read<Integer>("/tmp/ruff");

	CHECK(vals_in);
	std::vector<Integer> read_vals = *vals_in;


	for(size_t i = 0; i < vals.size(); ++i)
	{
		CHECK(read_vals[i] == vals[i]);
	}

}
