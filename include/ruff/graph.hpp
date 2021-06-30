#pragma once
#include <concepts> // derived_from
#include <array>   
#include <vector>  
#include <cstddef> // size_t

#include "logger.hpp"

namespace ruff
{
namespace graph
{

// You can inherit from this struct to use your own data
struct Node
{
private:
protected:
	size_t id{0};
public:
	constexpr Node(size_t id) : id(id) {}
	constexpr Node() {}

	constexpr size_t get() const noexcept { return id; };
};

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Graph using adjacency matrix
 */
/* ----------------------------------------------------------------------------*/
template<class T, size_t t_size> requires std::derived_from<T, Node>
class MGraph 
{
private:
	size_t m_size{t_size};
	std::array<std::array<size_t, t_size>, t_size> m_adjacent;

public:
	constexpr MGraph() : m_adjacent({})
	{
	};
	constexpr bool addEdge(size_t t_node1, size_t t_node2, int weight = 1)
	{
		if(t_node1 >= m_size || t_node2 >= m_size)
		{
			return false;
		}
		m_adjacent[t_node1][t_node2] = weight;
		m_adjacent[t_node2][t_node1] = weight;
		return true;
	};
	[[nodiscard]] std::vector<size_t> getEdges(const size_t vert) const noexcept
	{
		std::vector<size_t> edges{};
		for(size_t i = 0; i < m_size; ++i)
		{
			if(m_adjacent[vert][i] > 0)
			{
				edges.push_back(i);
			}
		}
		return edges;
	};
	[[nodiscard]] std::vector<std::vector<size_t>> getEdges() const noexcept
	{
		std::vector<std::vector<size_t>> edges{};
		for(size_t i = 0; i < m_size; ++i)
		{
			edges.push_back(getEdges(i));
		}
		return edges;
	};

	constexpr void print()
	{
		std::string matrix = "\n";
		for(size_t i = 0; i < m_size; ++i)
		{
			for(size_t j = 0; j < m_size; ++j)
			{
				matrix += std::to_string(m_adjacent[i][j]) + "\t";
			}
			matrix += "\n";
		}
		logInfo(matrix);
	};

	constexpr size_t size() const noexcept { return m_size; };
};
} // namespace graph
} // namespace ruff
