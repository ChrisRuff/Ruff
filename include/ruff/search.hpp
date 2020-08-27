#pragma once

// STD
#include <vector>   //std::vector
#include <array>    //std::array
#include <optional> //std::optional
#include <stack>    //std::stack
#include <cassert>  //assert()
#include <limits>   //std::numeric_limits<double>::max()

// Packages

// Source files
#include "point.hpp" // Point2D
#include "logger.hpp"
namespace ruff
{
	namespace search
	{
		constexpr double doubleMax = std::numeric_limits<double>::max();
		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis Struct that represents a node in the graph created by 
		 * searching algorithms
		 */
		/* ----------------------------------------------------------------------------*/
		struct Node 
		{
			int x{}; 
			int y{};
			int parentX{};
			int parentY{};

			// g - h - f
			std::array<double, 3> cost{doubleMax, doubleMax, doubleMax};

			Node(Point2D<int> pt) : x(pt.x), y(pt.y), parentX(0), parentY(0)
			{
			}
			Node() = default;
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
			inline friend bool operator<(const Node& lhs, const Node& rhs) 
			{
				return lhs.cost[2] < rhs.cost[2];
			}
		};


		/* --------------------------------------------------------------------------*/
		/**
		 * @Synopsis Class to solve the shortest path knowing the start and end positions
		 */
		/* ----------------------------------------------------------------------------*/
		class AStar 
		{ 
		private:
			const std::vector<std::vector<bool>> map{};

		public:
			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis  Begins a instance of the AStar algorithm that will be completed
			 * @Param map Region that will be searched within
			 */
			/* ----------------------------------------------------------------------------*/
			AStar(const std::vector<std::vector<bool>>& map);

			AStar() = default;

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
			 * @Synopsis Checks if a x and y are within the range of the map and is not a 
			 * wall
			 *
			 * @Param x 
			 * @Param y
			 *
			 * @Returns Whether or not the given x and y is a valid location on the given map
			 */
			/* ----------------------------------------------------------------------------*/
			inline bool isValid(const int& x, const int& y) 
			{
				if(x >= 0 && 
						y >= 0 && 
						x < static_cast<int>(this->map.size()) && 
						y < static_cast<int>(this->map[x].size()) && 
						this->map[x][y])
				{
					return true;
				}
				return false;
			}
			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis Calls the above function with the given point
			 *
			 * @Param pt 
			 *
			 * @Returns   
			 */
			/* ----------------------------------------------------------------------------*/
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
				if(x == end.x && y == end.y)
				{
					return true;
				}
				return false;
			}
			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis  Calculates Euclidian distance between x, y, and the end
			 *
			 * @Param x 
			 * @Param y
			 * @Param end End Node
			 *
			 * @Returns Euclidian distance between (x,y) and the end
			 */
			/* ----------------------------------------------------------------------------*/
			static inline double calculateH(const int& x, const int& y, const Node& end) 
			{
				// Euclidean distance from current position to end
				double H = ((x - end.x) * (x - end.x) +
						(y - end.y) * (y - end.y));
				return H;
			}

			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis Converts a list of nodes to a list of Point2D<int>-s
			 *
			 * @Param nodesOp List of Nodes that represent a path
			 *
			 * @Returns List of Point2D<int> showing the path
			 */
			/* ----------------------------------------------------------------------------*/
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
			/* --------------------------------------------------------------------------*/
			/**
			 * @Synopsis Follows the node path using the parent values to find the full path from the end node
			 * to the start node
			 *
			 * @Param mapping Map of nodes representing the board
			 * @Param end Destination being searched for 
			 *
			 * @Returns List of nodes that is the path from the start to end
			 */
			/* ----------------------------------------------------------------------------*/
			std::optional<std::vector<Node>> makePath(const std::vector<std::vector<Node>>& mapping, const Node& end);

		};
	};
#ifdef BUILD_TESTERS
	namespace tester
	{
		//ASTAR
		bool findEasyPath();
	};
#endif
};
