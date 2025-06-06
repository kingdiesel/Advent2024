// https://adventofcode.com/2024/day/15
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <map>
#include <regex>
//#define SAMPLE_INPUT
#ifdef SAMPLE_INPUT
constexpr int rows = 10;
constexpr int columns = 10 * 2;
#else
constexpr int rows = 50;
constexpr int columns = 50 * 2;
#endif
static char grid[rows][columns] = {};
static char grid_scratch[rows][columns] = {};

struct GridPoint
{
	GridPoint(int in_row, int in_column)
		: row(in_row), column(in_column) {}

	GridPoint() = default;

	bool IsValid() const
	{
		return row >= 0 && row < rows && column >= 0 && column < columns;
	}

	// call IsValid before using this function
	char GetValue() const
	{
		assert(IsValid());
		return grid[row][column];
	}

	void SetValue(char value)
	{
		assert(IsValid());
		grid[row][column] = value;
	}

	bool operator==(const GridPoint& other) const
	{
		return row == other.row && column == other.column;
	}

	bool operator!=(const GridPoint& other) const
	{
		return !(*this == other);
	}

	int row = 0;
	int column = 0;
};

struct Box
{
	GridPoint left_side;
	GridPoint right_side;

	bool operator==(const Box& other) const
	{
		return left_side == other.left_side && right_side == other.right_side;
	}

	bool operator!=(const Box& other) const
	{
		return !(*this == other);
	}

	bool ScratchMove(char move)
	{
		GridPoint next_left = left_side;
		GridPoint next_right = right_side;

		if (move == '^')
		{
			next_left.row--;
			next_right.row--;
		}
		else if (move == 'v')
		{
			next_left.row++;
			next_right.row++;
		}

		if (!next_left.IsValid() || !next_right.IsValid())
		{
			return false; // out of bounds
		}

		if (grid_scratch[next_left.row][next_left.column] != '.' ||
			grid_scratch[next_right.row][next_right.column] != '.')
		{
			return false; // blocked
		}

		grid_scratch[next_left.row][next_left.column] = '[';
		grid_scratch[next_right.row][next_right.column] = ']';

		grid_scratch[left_side.row][left_side.column] = '.';
		grid_scratch[right_side.row][right_side.column] = '.';

		return true;
	}

	void RealMove(char move)
	{
		GridPoint next_left = left_side;
		GridPoint next_right = right_side;

		if (move == '^')
		{
			next_left.row--;
			next_right.row--;
		}
		else if (move == 'v')
		{
			next_left.row++;
			next_right.row++;
		}

		grid[next_left.row][next_left.column] = '[';
		grid[next_right.row][next_right.column] = ']';

		grid[left_side.row][left_side.column] = '.';
		grid[right_side.row][right_side.column] = '.';
		left_side = next_left;
		right_side = next_right;
	}
};

void GetBoxesTouching(std::vector<Box>& boxes, const Box& current_box, char move)
{
	if (std::find(boxes.begin(), boxes.end(), current_box) != boxes.end())
	{
		return; // already processed this box
	}

	boxes.push_back(current_box);

	int row_modifier = -1;
	if (move == 'v')
	{
		row_modifier = 1;
	}
	GridPoint left_side_up(current_box.left_side.row + row_modifier, current_box.left_side.column);
	GridPoint right_side_up(current_box.right_side.row + row_modifier, current_box.right_side.column);

	if (left_side_up.GetValue() == ']')
	{
		Box new_box;
		new_box.right_side = left_side_up;
		new_box.left_side = GridPoint(left_side_up.row, left_side_up.column - 1);
		GetBoxesTouching(boxes, new_box, move);
	}
	else if (left_side_up.GetValue() == '[')
	{
		Box new_box;
		new_box.left_side = left_side_up;
		new_box.right_side = GridPoint(left_side_up.row, left_side_up.column + 1);
		GetBoxesTouching(boxes, new_box, move);
	}

	if (right_side_up.GetValue() == ']')
	{
		Box new_box;
		new_box.left_side = GridPoint(right_side_up.row, right_side_up.column - 1); 
		new_box.right_side = right_side_up;
		GetBoxesTouching(boxes, new_box, move);
	}
	else if (right_side_up.GetValue() == '[')
	{
		Box new_box;
		new_box.right_side = GridPoint(right_side_up.row, right_side_up.column + 1); 
		new_box.left_side = right_side_up;
		GetBoxesTouching(boxes, new_box, move);
	}
}


GridPoint robot_location;

void PrintGrid()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (robot_location.row == i && robot_location.column == j)
			{
				std::cout << '@';
			}
			else
			{
				std::cout << grid[i][j];
			}
		}
		std::cout << std::endl;
	}
}


void AdjustPointForMove(GridPoint& point, char move)
{
	if (move == '^')
	{
		point.row--;
	}
	else if (move == 'v')
	{
		point.row++;
	}
	else if (move == '<')
	{
		point.column--;
	}
	else if (move == '>')
	{
		point.column++;
	}
	else
	{
		assert(false);
	}
}

void PerformMoves(std::vector<char> moves)
{
	for (char move : moves)
	{
		GridPoint next_location = robot_location;
		AdjustPointForMove(next_location, move);

		assert(next_location.IsValid());
		if (next_location.GetValue() == '#')
		{
			continue;
		}
		else if (next_location.GetValue() == '.')
		{
			robot_location = next_location;
		}
		else if (next_location.GetValue() == 'O')
		{
			// get number of boxes in the move direction
			GridPoint loop_var = next_location;
			int line_length = 0;
			while (loop_var.GetValue() == 'O')
			{
				AdjustPointForMove(loop_var, move);
				line_length++;
			}

			if (loop_var.GetValue() == '#')
			{
				// line of boxes is blocked
				continue;
			}

			assert(loop_var.GetValue() == '.');

			// update the grid
			loop_var.SetValue('O');
			next_location.SetValue('.');

			// update robot position
			robot_location = next_location;
		}
		else
		{
			assert(false);
		}

		//std::cout << move << std::endl;
		//PrintGrid();
		//char c;
		//while (std::cin.get(c))
		//{
		//	break;
		//}
	}
}

void PerformMoves2(std::vector<char> moves)
{
	for (char move : moves)
	{
		GridPoint next_location = robot_location;
		AdjustPointForMove(next_location, move);

		assert(next_location.IsValid());
		if (next_location.GetValue() == '#')
		{
			continue;
		}
		else if (next_location.GetValue() == '.')
		{
			robot_location = next_location;
		}
		else if (next_location.GetValue() == '[' || next_location.GetValue() == ']')
		{
			if (move == '>' || move == '<')
			{
				// get number of boxes in the move direction
				GridPoint loop_var = next_location;
				int line_length = 0;
				while (loop_var.GetValue() == '[' || loop_var.GetValue() == ']')
				{
					AdjustPointForMove(loop_var, move);
					line_length++;
				}

				if (loop_var.GetValue() == '#')
				{
					// line of boxes is blocked
					continue;
				}

				assert(loop_var.GetValue() == '.');

				// update the grid
				loop_var.SetValue(next_location.GetValue() == '[' ? ']' : '[');
				GridPoint loop_var2 = next_location;
				while (loop_var2 != loop_var)
				{
					if (loop_var2.GetValue() == '[')
					{
						loop_var2.SetValue(']');
					}
					else if (loop_var2.GetValue() == ']')
					{
						loop_var2.SetValue('[');
					}
					AdjustPointForMove(loop_var2, move);
				}
				next_location.SetValue('.');

				// update robot position
				robot_location = next_location;
			}
			else if (move == '^' || move == 'v')
			{
				Box current_box;
				if (next_location.GetValue() == '[')
				{
					current_box.left_side = next_location;
					current_box.right_side = GridPoint(next_location.row, next_location.column + 1);
				}
				else
				{
					current_box.left_side = GridPoint(next_location.row, next_location.column - 1);
					current_box.right_side = next_location;
				}

				std::vector<Box> boxes;
				GetBoxesTouching(boxes, current_box, move);

				// sort the boxes by row
				if (move == '^')
				{
					std::sort(boxes.begin(), boxes.end(), [](const Box& a, const Box& b) {
						return a.left_side.row < b.left_side.row;
					});
				}
				else
				{
					std::sort(boxes.begin(), boxes.end(), [](const Box& a, const Box& b) {
						return a.left_side.row > b.left_side.row;
					});
				}

				// synchronize scratch grid
				memcpy(grid_scratch, grid, sizeof(grid_scratch));

				bool can_move_group = true;
				for (Box& box : boxes)
				{
					if (!box.ScratchMove(move))
					{
						can_move_group = false;
					}
				}

				if (can_move_group)
				{
					for (Box& box : boxes)
					{
						box.RealMove(move);
					}
					// update robot position
					robot_location = next_location;
				}
			}
		}
		else
		{
			assert(false);
		}

		//std::cout << move << std::endl;
		//PrintGrid();
		//char c;
		//while (std::cin.get(c))
		//{
		//	break;
		//}
	}
}

int main()
{
#ifdef SAMPLE_INPUT
    std::ifstream input_file("sample_input.txt");
#else
    std::ifstream input_file("input.txt");
#endif
    if (!input_file)
    {
        std::cerr << "Cannot open file.";
        return 1;
    }

	std::string line;
	int row = 0;
	char old_grid[rows][columns/2] = {};
	while (std::getline(input_file, line))
	{
		for (int i = 0; i < line.length(); i++)
		{
			old_grid[row][i] = line[i];
		}
		row++;

		if (row == rows)
		{
			break;
		}
	}

	// update to new grid
	/*
		If the tile is #, the new map contains ## instead.
		If the tile is O, the new map contains [] instead.
		If the tile is ., the new map contains .. instead.
		If the tile is @, the new map contains @. instead.
	*/
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			if (old_grid[i][j] == '#')
			{
				grid[i][j * 2] = '#';
				grid[i][(j * 2) + 1] = '#';
			}
			else if (old_grid[i][j] == 'O')
			{
				grid[i][j * 2] = '[';
				grid[i][(j * 2) + 1] = ']';
			}
			else if (old_grid[i][j] == '.')
			{
				grid[i][j * 2] = '.';
				grid[i][(j * 2) + 1] = '.';
			}
			else if (old_grid[i][j] == '@')
			{
				grid[i][j * 2] = '@';
				grid[i][(j * 2) + 1] = '.';
			}
		}
	}

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			if (grid[i][j] == '#')
			{
				grid_scratch[i][j] = '#';
			}
			else
			{
				grid_scratch[i][j] = '.';
			}
		}
	}

	std::string move_string;
	std::getline(input_file, line);
	while (std::getline(input_file, line))
	{
		move_string += line;
	}

	// remove all new lines
	move_string.erase(std::remove(move_string.begin(), move_string.end(), '\n'), move_string.end());
	move_string.erase(std::remove(move_string.begin(), move_string.end(), '\r'), move_string.end());

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (grid[i][j] == '@')
			{
				robot_location.row = i;
				robot_location.column = j;
				robot_location.SetValue('.');
				break;
			}
		}
	}

	std::vector<char> move_chars(move_string.begin(), move_string.end());
	PerformMoves2(move_chars);
	PrintGrid();
	//int64_t total_cost = 0;
	//for (int i = 0; i < rows; ++i)
	//{
	//	for (int j = 0; j < columns; ++j)
	//	{
	//		if (grid[i][j] == 'O')
	//		{
	//			int64_t cost = 100ll * i + j;
	//			total_cost += cost;
	//		}
	//	}
	//}

	//std::cout << "Total Cost: " << total_cost << std::endl;
	int total_cost = 0;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			if (grid[i][j] == '[')
			{
				total_cost += 100 * i + j;
			}
		}
	}

	std::cout << "Total Cost: " << total_cost << std::endl;
    return 0;
}