#include <ruff/ai/decision_tree.hpp>

#include <array>
#include <utility>
#include <iostream>

enum class TICTAC
{
	NONE = 0,
	O,
	X
};
std::string toString(TICTAC t)
{
	return t == TICTAC::NONE ? " " :
	       t == TICTAC::X    ? "X" :
	                           "O";
}
struct TicTacToeState
{
	std::array<std::array<TICTAC, 3>, 3> board{};
	TICTAC turn{};
	TICTAC winner{};
	int depth{ 0 };

	void Print() const
	{
		std::cout << "###########" << std::endl;
		for(int i = 0; i < 3; ++i)
		{
			std::cout << "#  ";
			for(int j = 0; j < 3; ++j)
			{
				std::cout << toString(board[i][j]) << " ";
			}
			std::cout << " #" << std::endl;
		}
		std::cout << "###########" << std::endl;
	}
};


int main()
{
	std::function ranker = [](TicTacToeState& state) -> std::pair<bool, double> {
		auto check_row = [state](int row) -> std::pair<bool, TICTAC> {
			bool winner = true;
			TICTAC start = state.board[row][0];
			if(start == TICTAC::NONE) return std::make_pair(false, start);
			for(int i = 1; i < 3; ++i)
			{
				winner &= state.board[row][i] == start;
			}
			return std::make_pair(winner, start);
		};
		auto check_col = [state](int col) -> std::pair<bool, TICTAC> {
			bool winner = false;
			TICTAC start = state.board[0][col];
			if(start == TICTAC::NONE) return std::make_pair(false, start);
			for(int i = 1; i < 3; ++i)
			{
				winner &= state.board[i][col] == start;
			}
			return std::make_pair(winner, start);
		};
		auto checker = [check_col, check_row, &state](int i) -> double {
			auto [won_row, winner_row] = check_row(i);
			auto [won_col, winner_col] = check_col(i);
			bool won = won_col || won_row;

			if(won)
			{
				state.winner = state.board[i][i];
				if(state.winner == TICTAC::X)
				{
					return 1.0 / state.depth;
				}
				else
				{
					return -1.0 / state.depth;
				}
			}
			return DBL_MAX;
		};
		for(int i = 0; i < 3; ++i)
		{
			double score = checker(i);
			if(score != DBL_MAX)
			{
				return std::make_pair(true, score);
			}
		}


		// diags
		bool won = false;
		won |= state.board[0][0] != TICTAC::NONE && state.board[0][0] == state.board[1][1] && state.board[1][1] == state.board[2][2];

		won |= state.board[0][2] != TICTAC::NONE && state.board[0][2] == state.board[1][1] && state.board[1][1] == state.board[2][0];
		if(won)
		{
			state.winner = state.board[1][1];
			if(state.winner == TICTAC::X)
			{
				return std::make_pair(true, 1 / state.depth);
			}
			else
			{

				return std::make_pair(true, -1 / state.depth);
			}
		}
		return std::make_pair(false, 0);
	};

	std::function generator = [](TicTacToeState state) -> std::vector<TicTacToeState> {
		std::vector<TicTacToeState> states;
		for(int i = 0; i < 3; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				if(state.board[i][j] == TICTAC::NONE)
				{
					auto test = state;
					test.turn = (test.turn == TICTAC::X) ? TICTAC::O : TICTAC::X;
					test.depth = state.depth + 1;
					test.board[i][j] = test.turn;
					states.push_back(test);
				}
			}
		}
		return states;
	};

	TicTacToeState initial;
	initial.board[0].fill(TICTAC::NONE);
	initial.board[1].fill(TICTAC::NONE);
	initial.board[2].fill(TICTAC::NONE);

	ruff::ai::DecisionTree<TicTacToeState> tree(generator, ranker, initial);

	std::function ai_func = [](std::vector<Tree<TicTacToeState>::TreeStructure>& nodes) noexcept -> Tree<TicTacToeState>::TreeStructure& {
		int max_idx = 0;
		for(size_t i = 0; i < nodes.size(); ++i)
		{
			if(nodes[i].value > nodes[max_idx].value)
			{
				max_idx = i;
			}
		}
		return nodes[max_idx];
	};
	std::function player_func = [tree](std::vector<Tree<TicTacToeState>::TreeStructure> nodes) -> Tree<TicTacToeState>::TreeStructure& {
		int row;
		int col;
		do
		{
			std::cout << "Enter your move (row column - \"0 0\")\n";
			std::cin >> row >> col;
		} while(tree.GetState().board[row][col] != TICTAC::NONE);
		for(auto& node : nodes)
		{
			if(node.data.board[row][col] != TICTAC::NONE)
				return node;
		}
		throw "End of game";
	};

	tree.MiniMax();
	while(!tree.Finished())
	{
		tree.Move(ai_func);
		tree.Print();

		if(tree.Finished())
		{
			break;
		}
		tree.Move(player_func);
	}
	tree.Print();
}
