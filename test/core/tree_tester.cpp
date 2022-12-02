//
// Created by chris on 2021-11-08.
//
#include <iostream>
#include "doctest/doctest.h"
#include "ruff/core/structs/tree.hpp"

TEST_SUITE("Tree Tests")
{
	TEST_CASE("Make a Tree")
	{
		ruff::data::Tree<ruff::data::TreeNode> tree{};
		auto root = tree.getRoot();
		root->addChild({ 1 });
		root->addChild({ 2 });

		CHECK(root->get() == 0);

		auto children = root->getChildren();
		for(size_t i = 0; i < children.size(); ++i)
		{
			CHECK(children[i].get() == i + 1);
		}
	}
	TEST_CASE("BFS")
	{
		ruff::data::Tree<ruff::data::TreeNode> tree{};
		auto root = tree.getRoot();
		root->addChild({ 1 });
		root->getChildren()[0].addChild({ 2 });
		root->addChild({ 3 });

		std::vector<ruff::data::TreeNode*> node_list = tree.BFS();
		CHECK(node_list[0]->get() == 0);
		CHECK(node_list[1]->get() == 1);
		CHECK(node_list[2]->get() == 3);
		CHECK(node_list[3]->get() == 2);
	}
	TEST_CASE("Print")
	{
		ruff::data::Tree<ruff::data::TreeNode> tree{};
		ruff::data::TreeNode* root = tree.getRoot();
		root->addChild({ 1 });
		root->getChildren()[0].addChild({ 2 });
		root->getChildren()[0].addChild({ 5 });
		root->addChild({ 3 });
		root->addChild({ 4 });
		root->getChildren()[2].addChild({ 2 });
		root->getChildren()[2].addChild({ 5 });

		std::string tree_string = static_cast<std::string>(tree);
        std::string tree_string_gt = "0 \n1 3 4 \n2 5 2 5 \n";
        CHECK(tree_string == tree_string_gt);
	}
}
