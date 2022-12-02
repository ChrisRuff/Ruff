//
// Created by chris on 2021-10-25.
//

#pragma once
#include <concepts>
#include <string>
#include <queue>

#include "ruff/core/structs/data.hpp"

namespace ruff
{
namespace data
{

	template<class T>
	requires std::derived_from<T, TreeNode>
	class Tree
	{
	private:
		T root{};

	public:
		Tree() = default;

		T* getRoot() { return &root; };

		std::vector<T*> BFS()
		{
			std::vector<T*> output{};
			std::queue<T*> queue{};
			queue.push(&root);
			while(!queue.empty())
			{
				T* current = queue.front();
				std::vector<T>& children = current->getChildren();
				output.push_back(current);

				queue.pop();
				for(size_t i = 0; i < children.size(); ++i)
				{
					queue.emplace(&(children[i]));
				}
			}
			return output;
		}

		std::vector<std::vector<T*>> node_list()
		{
			auto ordered_list = BFS();
			std::vector<std::vector<T*>> node_list{};
			for(size_t i = 0; i < ordered_list.size(); ++i)
			{
				auto node = ordered_list[i];
				if(node_list.size() <= node->getDepth())
				{
					node_list.push_back({ node });
				}
				else
				{
					node_list[node->getDepth()].push_back(node);
				}
			}
			return node_list;
		}


		operator std::string()
		{
			std::string out{};
			auto node_list = this->node_list();
			for(size_t i = 0; i < node_list.size(); ++i)
			{
				for(size_t j = 0; j < node_list[i].size(); ++j)
				{
					out += std::to_string(node_list[i][j]->get());
					out += " ";
				}
				out += "\n";
			}
			return out;
		};
		friend std::ostream& operator<<(std::ostream& str, Tree<T> tree)
		{
			str << static_cast<std::string>(tree);
			return str;
		};
	};
}// namespace data
}// namespace ruff
