#pragma once
#include <queue>

#include <ruff/ai/node.hpp>

template<typename T>
class Tree
{
public:
	struct TreeStructure
	{
		double value{};

		std::vector<TreeStructure> children{};
		T data{};

		TreeStructure() = default;
		TreeStructure(T data) noexcept : data(data){};
		operator T() const { return data; }
	};

protected:
	TreeStructure root{};

public:
	Tree() = default;

	template<typename OP>
	void DFS(Tree::TreeStructure& node, OP operation)
	{
		if(node.children.empty())
			return;
		for(auto& child : root.children)
		{
			DFS(child, operation);
		}
		operation(node);
	}

	template<typename OP>
	void DFS(OP operation)
	{
		DFS(root, operation);
	}

	template<typename OP>
	void BFS(OP operation)
	{
		std::queue<TreeStructure*> queue;
		for(auto& child : root.children)
		{
			queue.push(&child);
		}
		while(!queue.empty())
		{
			auto& current = *(queue.front());
			operation(current);

			for(auto& child : current.children)
			{
				queue.push(&child);
			}
			queue.pop();
		}
	}
};