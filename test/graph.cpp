#include "doctest/doctest.h"
#include "graph.hpp"

TEST_SUITE("Graphs")
{
	TEST_CASE("Creation of an adjacency matrix")
	{
		ruff::graph::MGraph<ruff::graph::Node, 5> graph{};

		graph.addEdge(1,2);
		graph.addEdge(5,6);
		graph.print();

	}
}
