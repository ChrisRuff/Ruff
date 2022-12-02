#pragma once
#include <vector>
#include <algorithm>
#include <cfloat>
#include <functional>
#include <iostream>

namespace ruff::ai
{

template<typename ST>
class Node
{
public:
	Node* m_parent;
	std::vector<Node> m_children;

	ST m_state;

	double m_value{ 0 };
	double m_probability;


public:
	Node(ST state,
	     std::function<std::vector<ST>(ST)>& generator,
	     std::function<bool(ST&)>& checker,
	     std::function<double(ST)>& ranker,
	     Node* parent,
	     double probability = 0) : m_state(state),
	                               m_parent(parent),
	                               m_probability(probability)
	{
		auto states = generator(m_state);
		if(!checker(state))
		{
			for(auto& state : states)
			{
				m_children.emplace_back(state, generator, checker, ranker, this);
			}
		}
		if(m_children.size() == 0)
		{
			m_value = ranker(state);
		}
	}

	bool operator<(const Node<ST>& other) { return m_value < other.m_value; }
	void Print()
	{
		std::cout << std::to_string(m_value) << std::endl;
		m_state.Print();
		std::cout << std::endl;
	}
};
}// namespace ruff::ai