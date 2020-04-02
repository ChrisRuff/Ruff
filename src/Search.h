#pragma once

// STD
#include <vector>
#include <array>
#include <string>
#include <optional> 
#include <cmath>  // sqrt
#include <cfloat> // DBL_MAX
#include <stack>
#include <cassert>

#ifndef NDEBUG
#include <iostream>
#endif

// Packages

// Source files
#include "Point.h" // Point2D
namespace MLibs
{
	namespace Search
	{
		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis Struct that represents a node in the graph created by 
		 * searching algorithms
		 */
		/* ----------------------------------------------------------------------------*/
		struct Node 
		{
			int x; 
			int y;
			int parentX;
			int parentY;

			// g - h - f
			std::array<float, 3> cost;

			Node(Point2D<int> pt) : x(pt.x), y(pt.y)
			{
			}
			Node() 
			{
				x = -1;
				y = -1;
				parentX = -1;
				parentY = -1;
			}
		};

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis  Compares two nodes by returning f costs
		 *
		 * @Param lhs Left node
		 * @Param rhs Right node
		 *
		 * @Returns Whether or not lhs is less than rhs 
		 */
		/* ----------------------------------------------------------------------------*/
		inline bool operator<(const Node& lhs, const Node& rhs) 
		{
			return lhs.cost[2] < rhs.cost[2];
		}

		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis Class to solve the shortest path knowing the start and end positions
		 */
		/* ----------------------------------------------------------------------------*/
		class AStar 
		{ 
		private:
			const std::vector<std::vector<bool>> map;

		public:
			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis  Begins a instance of the AStar algorithm that will be completed
			 * @Param map Region that will be searched within
			 */
			/* ----------------------------------------------------------------------------*/
			AStar(const std::vector<std::vector<bool>>& map);

			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis  Finds the shortest path from the start to the end in the map given
			 * in the construction of the AStar object
			 *
			 * @Param start Where the search began from 
			 * @Param end Where the search tries to reach
			 *
			 * @Returns Optional variable containing the shortest path from start to end
			 */
			/* ----------------------------------------------------------------------------*/
			std::optional<std::vector<Point2D<int>>> getPath(const Point2D<int>& start, 
				                                               const Point2D<int>& end);

			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis  Convenience method for the above
			 *
			 * @Param x1 Start x coordinate
			 * @Param y1 Start y coordinate
			 * @Param x2 End x coordinate
			 * @Param y2 End y coordinate
			 *
			 * @Returns Optional variable containing the shortest path from start to end
			 */
			/* ----------------------------------------------------------------------------*/
			std::optional<std::vector<Point2D<int>>> getPath(const int& x1, const int& y1, 
				                                             const int& x2, const int& y2);


			// #############################/ HELPER FUNCTIONS \###############################
			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis  
			 *
			 * @Param x
			 * @Param y
			 *
			 * @Returns   
			 */
			/* ----------------------------------------------------------------------------*/
			inline bool isValid(const int& x, const int& y) 
			{
				if(x >= 0 && x < this->map.size() && y >= 0 && y < this->map[x].size() && this->map[x][y])
				{
					return true;
				}
				return false;
			}
			inline bool isValid(const Point2D<int> pt)
			{
				return isValid(pt.x, pt.y);
			}
			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis  Returns if the the current positions are the end node
			 *
			 * @Param x Test location x
			 * @Param y Test location y
			 * @Param end Final node
			 *
			 * @Returns Whether or not node at x, y is the end  
			 */
			/* ----------------------------------------------------------------------------*/
			static inline bool isEnd(const int& x, const int& y, const Node& end) 
			{
				std::cout << x << " " << y << std::endl;
				if(x == end.x && y == end.y)
				{
					return true;
				}
				return false;
			}
			static inline double calculateH(const int& x, const int& y, const Node& end)
			{
				// Euclidean distance from current position to end
				double H = (std::sqrt((x - end.x) * (x - end.x) +
						(y - end.y) * (y - end.y)));
				return H;
			}
			static inline std::vector<Point2D<int>> convertToPoints(const std::optional<std::vector<Node>>& nodesOp)
			{
				if(nodesOp)
				{
					auto nodes = nodesOp.value();
					std::vector<Point2D<int>> points(nodes.size());
					for(size_t i = 0; i < nodes.size(); ++i)
					{
						points[i] = Point2D<int>(nodes[i].x, nodes[i].y);
					}
					return points;
				}
				else
				{
#ifndef NDEBUG
					std::cout << "Vector of nodes contain no values" << std::endl;
#endif
					return std::vector<Point2D<int>>{};
				}
			}
		std::optional<std::vector<Node>> makePath(const std::vector<std::vector<Node>>& mapping, const Node& end);
		};
	};
	namespace Tester
	{
		bool findEasyPath();
		void testAStar();
		void testSearch();
	};
};
