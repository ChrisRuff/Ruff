#pragma once
#include <cstddef>
#include <vector>
namespace ruff
{
namespace data
{
	// You can inherit from this struct to use your own data
	struct Node
	{
	private:
	protected:
		// Id is set when added to graphs
		size_t id;

	public:
		constexpr Node(size_t id = 0) noexcept : id(id) {}
		constexpr size_t get() const noexcept { return id; };
	};
	struct TreeNode : public Node
	{
	private:
		TreeNode* parent;
		std::vector<TreeNode> children{};
		size_t depth{ 0 };

	public:
		TreeNode(size_t id = 0, TreeNode* parent = nullptr) : Node(id), parent(parent){};

		TreeNode(const TreeNode&) = default;
		TreeNode& operator=(const TreeNode&) = delete;

		void addChild(TreeNode child)
		{
			child.setParent(this);
			children.push_back(child);
		};

		void setParent(TreeNode* parent_node)
		{
			this->parent = parent_node;
			depth = parent_node->getDepth() + 1;
		}

		TreeNode* getParent() const { return parent; };
		std::vector<TreeNode>& getChildren() { return children; };
		size_t getDepth() { return depth; };
	};
};// namespace data
};// namespace ruff
