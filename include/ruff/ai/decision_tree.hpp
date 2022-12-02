#pragma once
#include <functional>
#include <vector>

#include <ruff/ai/node.hpp>
#include <ruff/ai/tree.hpp>

namespace ruff::ai
{
template<typename T>
class DecisionTree : public Tree<T>
{
public:
	DecisionTree(std::function<std::vector<T>(T)> generator,
	             std::function<std::pair<bool, double>(T&)> ranker,
	             T initial) noexcept
	{
		Tree<T>::root.data = initial;
		auto func = [generator, ranker](typename Tree<T>::TreeStructure& tree) -> void {
			auto states = generator(tree);
			if(states.empty())
				return;
			for(auto state : states)
			{
				tree.children.emplace_back(state);
			}
		};
		func(Tree<T>::root);
		Tree<T>::BFS(func);

		Tree<T>::DFS([ranker](typename Tree<T>::TreeStructure& state) {
			auto rank = ranker(state.data);
			if(rank.first)
			{
				state.value = rank.second;
			}
		});
	}
	void MiniMax()
	{
		Tree<T>::DFS([](typename Tree<T>::TreeStructure& state) {
				(void)state;
		});
	}
	bool Finished() { return Tree<T>::root.children.empty(); };
	void Move(std::function<typename Tree<T>::TreeStructure&(
	            std::vector<typename Tree<T>::TreeStructure>&)> move_func)
	{
		auto& next = move_func(Tree<T>::root.children);
		Tree<T>::root = next;
	}
	const T& GetState() const { return Tree<T>::root.data; };
	void Print()
	{
		Tree<T>::root.data.Print();
	}
};
}// namespace ruff::ai