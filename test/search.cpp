#include "doctest/doctest.h"
#include <iostream>

#include "ruff/core/search.hpp"

using namespace ruff;

bool findEasyPath()
{
	std::vector<std::vector<bool>> map = { { false, false, true, false, false, true },
																				 { false, true, true, false, true, true },
																				 { true, true, false, false, true, false },
																				 { true, true, true, true, true, true } };
	std::vector<Point2D<int>> rightPath = { 
		Point2D<int>(0, 2), 
		Point2D<int>(1, 2), 
		Point2D<int>(2, 1), 
		Point2D<int>(3, 2), 
		Point2D<int>(3, 3), 
		Point2D<int>(2, 4), 
		Point2D<int>(1, 5), 
		Point2D<int>(0, 5) 
	};

	Point2D<int> start(0, 2), end(0, 5);
	ruff::search::AStar searcher(map);
	auto pathRet = searcher.getPath(start, end);
	if(pathRet)
	{
		auto path = pathRet.value();
		if(path.size() != rightPath.size())
		{
			return false;
		}
		for(size_t i = 0; i < path.size(); ++i)
		{
			if(path[i] != rightPath[i])
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
};

TEST_CASE("Testing AStar Search")
{
	CHECK(findEasyPath());
};
