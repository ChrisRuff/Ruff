#pragma once
#include <concepts>// derived_from
#include <array>
#include <vector>
#include <algorithm>// find
#include <map>
#include <cstddef>// size_t

#include <ruff/core/structs/data.hpp>
#include "ruff/core/logger.hpp"

namespace ruff
{
namespace data
{

	/* --------------------------------------------------------------------------*/
	/**
	 * @Synopsis  Undirected graph using adjacency List
	 */
	/* ----------------------------------------------------------------------------*/
	template<class T>
	requires std::derived_from<T, Node>
	class Graph
	{
	private:
		std::vector<T> nodes{};
		std::map<size_t, std::vector<T*>> adjacency{};

	public:
		Graph() = default;

		template<typename... Args>
		T& addNode(Args... args)
		{
			T node(nodes.size(), args...);
			nodes.emplace_back(nodes.size(), args...);

			return nodes[nodes.size() - 1];
		}
		void addEdge(size_t n1, size_t n2)
		{
			adjacency[n1].push_back(&nodes[n2]);
			adjacency[n2].push_back(&nodes[n1]);
		}
		void addEdge(Node& n1, Node& n2)
		{
			adjacency[n1.get()].push_back(&n2);
			adjacency[n2.get()].push_back(&n1);
		}
		std::vector<T*> DFS(const T& n)
		{
			std::vector<bool> visited(nodes.size(), false);
			return DFS(n, visited);
		}
		std::vector<T*> DFS(const T& n, std::vector<bool>& visited)
		{
			std::vector<T*> elements{ &nodes[n.get()] };
			visited[n.get()] = true;
			for(const T* node : adjacency[n.get()])
			{
				if(!visited[node->get()])
				{
					std::vector<T*> subelements = DFS(*node, visited);
					elements.insert(elements.end(), subelements.begin(), subelements.end());
				}
			}
			return elements;
		}
		std::vector<std::vector<T*>> getIslands()
		{
			std::vector<std::vector<T*>> islands{};
			std::vector<bool> visited(nodes.size(), false);
			T target = 0;
			std::vector<bool>::iterator it = visited.begin();
			while(it != visited.end())
			{
				islands.push_back(DFS(target, visited));
				it = std::find(visited.begin(), visited.end(), false);
				if(it != visited.end())
					target = nodes[std::distance(visited.begin(), it)];
			}
			return islands;
		}

		T getNode(const size_t id) const { return nodes[id]; }
		T& getNode(const size_t id) { return nodes[id]; }
	};
}// namespace data
}// namespace ruff
