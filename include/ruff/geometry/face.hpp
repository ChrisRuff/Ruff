#pragma once
#include <vector>
#include <type_traits>

#include "ruff/geometry/point.hpp"
#include "ruff/geometry/line.hpp"

namespace ruff
{
namespace geometry
{
	class Face
	{
	private:
		std::vector<Line> edges{};

	public:
		Face() = default;
		template<typename It> 
			requires std::is_same_v<size_t, typename std::iterator_traits<It>::value_type>
		Face(const It begin, const It end)
		{
			auto it = begin;
			while(it != (end-1))
			{
				edges.emplace_back(*it, *(it+1));
				++it;
			}
			edges.emplace_back(*it, *begin);
		}
		std::vector<size_t> getVerts() const
		{
			std::vector<size_t> idxs;
			for(const auto& line : edges)
			{
				idxs.push_back(line.start());
			}
			return idxs;
		}

		Line edge(size_t idx) const { return edges[idx]; };
		size_t nedges() const { return edges.size(); };
	};
};
};
