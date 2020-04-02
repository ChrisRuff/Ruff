#include "Search.h"

namespace MLibs
{
	namespace Search
	{
		AStar::AStar(const std::vector<std::vector<bool>>& map) : map(map)
		{
		}
		std::optional<std::vector<Point2D<int>>> AStar::getPath(const Point2D<int>& start, 
				                                             const Point2D<int>& end)
		{
			Node endNode(end);
			std::vector<Node> empty;
			if(!this->isValid(end))
			{
#ifndef NDEBUG
				std::cout << "Destination is an obstacle" << std::endl;
#endif
				return std::nullopt;
			}
			if(isEnd(start.x, start.y, endNode))
			{
#ifndef NDEBUG
				std::cout << "You are the destination" << std::endl;
#endif
				return std::nullopt;
			}
			std::vector<std::vector<bool>> closed_list(map.size());
			std::vector<std::vector<Node>> allMap(map.size());
			for(size_t x = 0; x < map.size(); ++x)
			{
				allMap[x] = std::vector<Node>(map[x].size());
				closed_list[x] = std::vector<bool>(map[x].size());
				for(size_t y = 0; y < map[x].size(); ++y)
				{
					allMap[x][y].cost[2] = DBL_MAX;
					allMap[x][y].cost[1] = DBL_MAX;
					allMap[x][y].cost[0] = DBL_MAX;
					allMap[x][y].parentX = -1;
					allMap[x][y].parentY = -1;
					allMap[x][y].x = x;
					allMap[x][y].y = y;
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
			bool endFound = false;

			while(!open_list.empty())
			{
				Node node;
				do 
				{
					float temp = DBL_MAX;
					std::vector<Node>::iterator itNode;
					for(std::vector<Node>::iterator it = open_list.begin();
							it != open_list.end(); it = next(it))
					{
						Node n = *it;
						if(n.cost[2] < temp)
						{
							temp = n.cost[2];
							itNode = it;
						}
					}
					node = *itNode;
					open_list.erase(itNode);
				}while(!isValid(node.x, node.y));

				x = node.x;
				y = node.y;
				closed_list[x][y] = true;

				for(int xNew = -1; xNew <= 1; ++xNew)
				{
					for(int yNew = -1; yNew <= 1; ++yNew)
					{
						double gNew, hNew, fNew;
						if(isValid(x+xNew, y+yNew))
						{
							if(isEnd(x+xNew, y+yNew, endNode))
							{
								allMap[x+xNew][y+yNew].parentX = x;
								allMap[x+xNew][y+yNew].parentY = y;
								endFound = true;
								auto path = makePath(allMap, endNode);
								return std::optional<std::vector<Point2D<int>>>{convertToPoints(path)};
							}
							else if(!closed_list[x+xNew][y+yNew])
							{
								std::cout << "Checking: " << Point2D<int>(x+xNew, y+yNew) << std::endl;
								gNew = node.cost[0] + 1.0;
								hNew = calculateH(x+xNew, y+yNew, end);
								fNew = gNew + hNew;
								if(allMap[x+xNew][y+yNew].cost[2] == DBL_MAX ||
										allMap[x+xNew][y+yNew].cost[2] > fNew)
								{
									allMap[x+xNew][y+yNew].cost[2] = fNew;
									allMap[x+xNew][y+yNew].cost[1] = hNew;
									allMap[x+xNew][y+yNew].cost[0] = gNew;
									allMap[x+xNew][y+yNew].parentX = x;
									allMap[x+xNew][y+yNew].parentY = y;
									open_list.emplace_back(allMap[x+xNew][y+yNew]);
								}
							}
						}
					}
				}
			}
#ifndef NDEBUG
				std::cout << "Destination was not found" << std::endl;
#endif
				return std::nullopt;
		}
		std::optional<std::vector<Point2D<int>>> AStar::getPath(const int& x1, const int& y1, 
				                                             const int& x2, const int& y2)
		{
			Point2D<int> start(x1, y1);
			Point2D<int> end(x2, y2);

			return getPath(start, end);
		}

		std::optional<std::vector<Node>> AStar::makePath(const std::vector<std::vector<Node>>& mapping, const Node& end)
		{
			try
			{
				int x = end.x;
				int y = end.y;
				std::stack<Node> path;
				std::vector<Node> usablePath;
				
				while(!(mapping[x][y].parentX == x && mapping[x][y].parentY == y) &&
						mapping[x][y].x != -1 && mapping[x][y].y != -1)
				{
					path.push(mapping[x][y]);
					int tempX = mapping[x][y].parentX;
					int tempY = mapping[x][y].parentY;
					x = tempX;
					y = tempY;
				}
				path.push(mapping[x][y]);

				while(!path.empty())
				{
					Node top = path.top();
					path.pop();
					usablePath.emplace_back(top);
				}
				return std::optional<std::vector<Node>>{usablePath};
			}
			catch(const std::exception& el)
			{
				std::cout << el.what() << std::endl;
				return std::nullopt;
			}
		}
	}
	namespace Tester
	{
		bool findEasyPath()
		{                                                     //Start               //End
			std::vector<std::vector<bool>> map = {{ false, false, true,  false, false, true  },
																						{ false, true,  true,  false, true,  true  },
																						{ true,  true,  false, false, true,  false },
																						{ true,  true,  true,  true,  true,  true  }};
			Point2D<int> start(0,2), end(0,5);
			Search::AStar searcher(map);
			auto pathRet = searcher.getPath(start, end);
			assert(pathRet);

			return true;
		}
		void testAStar()
		{
			assert (findEasyPath());
		}
		void testSearch()
		{
			testAStar();
		}
	}
}
