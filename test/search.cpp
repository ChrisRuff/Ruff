#include "doctest/doctest.h"
#include <iostream>

#include "ruff/core/search.hpp"

using namespace ruff::tester;

TEST_CASE("Testing AStar Search")
{
	CHECK(findEasyPath());
}
