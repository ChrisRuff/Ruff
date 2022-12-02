//
// Created by chris on 2021-10-27.
//
#include "doctest/doctest.h"

#include "ruff/core/structs/graph.hpp"
TEST_SUITE("Graph Tests")
{
	TEST_CASE("Create and populate Adjacency List")
	{
		ruff::data::Graph<ruff::data::Node> my_graph{};
		my_graph.addNode();
		auto n2 = my_graph.addNode();
		auto n3 = my_graph.addNode();

		my_graph.addEdge(0, 1);
		my_graph.addEdge(n2, n3);
	}
	TEST_CASE("DFS")
	{
		ruff::data::Graph<ruff::data::Node> my_graph{};
		auto n0 = my_graph.addNode();
		auto n1 = my_graph.addNode();
		auto n2 = my_graph.addNode();
		auto n3 = my_graph.addNode();
		my_graph.addNode();

		my_graph.addEdge(0, 1);
		my_graph.addEdge(n1, n2);
		my_graph.addEdge(2, 4);
		my_graph.addEdge(4, 3);

		auto node_list = my_graph.DFS(n0);
		CHECK(node_list[0]->get() == 0);
		CHECK(node_list[1]->get() == 1);
		CHECK(node_list[2]->get() == 2);
		CHECK(node_list[3]->get() == 4);
		CHECK(node_list[4]->get() == 3);

		node_list = my_graph.DFS(n3);
		CHECK(node_list[0]->get() == 3);
		CHECK(node_list[1]->get() == 4);
		CHECK(node_list[2]->get() == 2);
		CHECK(node_list[3]->get() == 1);
		CHECK(node_list[4]->get() == 0);
	}
	TEST_CASE("Islands")
	{
		ruff::data::Graph<ruff::data::Node> my_graph{};
		my_graph.addNode();
		my_graph.addNode();
		my_graph.addNode();
		my_graph.addNode();
		my_graph.addNode();

		my_graph.addEdge(0, 1);
		my_graph.addEdge(2, 3);

		auto node_list = my_graph.getIslands();
		CHECK(node_list[0].size() == 2);
		CHECK(node_list[1].size() == 2);
		CHECK(node_list[2].size() == 1);

		CHECK(node_list[0][0]->get() == 0);
		CHECK(node_list[0][1]->get() == 1);

		CHECK(node_list[1][0]->get() == 2);
		CHECK(node_list[1][1]->get() == 3);

		CHECK(node_list[2][0]->get() == 4);
	}
}