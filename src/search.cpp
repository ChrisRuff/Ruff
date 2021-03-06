#include "search.hpp"

namespace ruff
{
namespace search
{
	AStar::AStar(const std::vector<std::vector<bool>>& map) : map(map)
	{
	}
	std::optional<std::vector<Point2D<int>>> AStar::getPath(const Point2D<int>& start, const Point2D<int>& end)
	{
		Node              endNode(end);
		std::vector<Node> empty;
		if(!this->isValid(end))
		{
			ruff::logWarning("Destination is an obstacle");
			return std::nullopt;
		}
		if(!this->isValid(start))
		{
			ruff::logWarning("Start is an obstacle");
			return std::nullopt;
		}
		if(isEnd(start.x, start.y, endNode))
		{
			logWarning("You are the destination");
			return std::nullopt;
		}
		std::vector<std::vector<bool>> closed_list(map.size());
		std::vector<std::vector<Node>> allMap(map.size());
		for(size_t x = 0; x < map.size(); ++x)
		{
			allMap[x]      = std::vector<Node>(map[x].size());
			closed_list[x] = std::vector<bool>(map[x].size());
			for(size_t y = 0; y < map[x].size(); ++y)
			{
				allMap[x][y].x    = x;
				allMap[x][y].y    = y;
				closed_list[x][y] = false;
			}
		}
		int x = start.x;
		int y = start.y;

		allMap[x][y].cost[2] = 0;
		allMap[x][y].cost[1] = 0;
		allMap[x][y].cost[0] = 0;
		allMap[x][y].parentX = x;
		allMap[x][y].parentY = y;

		std::vector<Node> open_list;
		open_list.emplace_back(allMap[x][y]);
		while(!open_list.empty())
		{
			Node node;
			do
			{
				double                      temp = doubleMax;
				std::vector<Node>::iterator itNode;
				for(std::vector<Node>::iterator it = open_list.begin();
				    it != open_list.end();
				    it = next(it))
				{
					Node n = *it;
					if(n.cost[2] < temp)
					{
						temp   = n.cost[2];
						itNode = it;
					}
				}
				node = *itNode;
				open_list.erase(itNode);
			} while(!isValid(node.x, node.y));

			x                 = node.x;
			y                 = node.y;
			closed_list[x][y] = true;

			for(int xNew = -1; xNew <= 1; ++xNew)
			{
				for(int yNew = -1; yNew <= 1; ++yNew)
				{
					double gNew, hNew, fNew;
					if(isValid(x + xNew, y + yNew))
					{
						if(isEnd(x + xNew, y + yNew, endNode))
						{
							allMap[x + xNew][y + yNew].parentX = x;
							allMap[x + xNew][y + yNew].parentY = y;
							auto path                          = makePath(allMap, endNode);
							return std::optional<std::vector<Point2D<int>>>{ convertToPoints(path) };
						}
						else if(!closed_list[x + xNew][y + yNew])
						{
							gNew = node.cost[0] + 1.0;
							hNew = calculateH(x + xNew, y + yNew, end);
							fNew = gNew + hNew;
							if(allMap[x + xNew][y + yNew].cost[2] == doubleMax || allMap[x + xNew][y + yNew].cost[2] > fNew)
							{
								allMap[x + xNew][y + yNew].cost[2] = fNew;
								allMap[x + xNew][y + yNew].cost[1] = hNew;
								allMap[x + xNew][y + yNew].cost[0] = gNew;
								allMap[x + xNew][y + yNew].parentX = x;
								allMap[x + xNew][y + yNew].parentY = y;
								open_list.emplace_back(allMap[x + xNew][y + yNew]);
							}
						}
					}
				}
			}
		}
		logWarning("Destination was not found");
		return std::nullopt;
	}
	std::optional<std::vector<Point2D<int>>> AStar::getPath(const int& x1, const int& y1, const int& x2, const int& y2)
	{
		Point2D<int> start(x1, y1);
		Point2D<int> end(x2, y2);

		return getPath(start, end);
	}

	std::optional<std::vector<Node>> AStar::makePath(const std::vector<std::vector<Node>>& mapping, const Node& end)
	{
		try
		{
			int               x = end.x;
			int               y = end.y;
			std::stack<Node>  path;
			std::vector<Node> usablePath;

			while(!(mapping[x][y].parentX == x && mapping[x][y].parentY == y) && mapping[x][y].x != -1 && mapping[x][y].y != -1)
			{
				path.push(mapping[x][y]);
				int tempX = mapping[x][y].parentX;
				int tempY = mapping[x][y].parentY;
				x         = tempX;
				y         = tempY;
			}
			path.push(mapping[x][y]);

			while(!path.empty())
			{
				Node top = path.top();
				path.pop();
				usablePath.emplace_back(top);
			}
			return std::optional<std::vector<Node>>{ usablePath };
		}
		catch(const std::exception& el)
		{
			logWarning(el.what());
			return std::nullopt;
		}
	}
}// namespace search
#ifdef BUILD_TESTERS
namespace tester
{
	bool findEasyPath()
	{//Start               //End
		std::vector<std::vector<bool>> map       = { { false, false, true, false, false, true },
                                           { false, true, true, false, true, true },
                                           { true, true, false, false, true, false },
                                           { true, true, true, true, true, true } };
		std::vector<Point2D<int>>      rightPath = { Point2D<int>(0, 2), Point2D<int>(1, 2), Point2D<int>(2, 1), Point2D<int>(3, 2), Point2D<int>(3, 3), Point2D<int>(2, 4), Point2D<int>(1, 5), Point2D<int>(0, 5) };

		Point2D<int>  start(0, 2), end(0, 5);
		search::AStar searcher(map);
		auto          pathRet = searcher.getPath(start, end);
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
	}
}// namespace tester
#endif
}// namespace ruff
