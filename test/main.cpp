#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Search.hpp"

using namespace Ruff::Tester;

TEST_CASE("Testing AStar Search")
{
	CHECK(findEasyPath());
}
