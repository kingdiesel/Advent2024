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
constexpr int columns = 10;
#else
constexpr int rows = 50;
constexpr int columns = 50;
#endif
static char grid[rows][columns] = {};

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

	int row = 0;
	int column = 0;
};
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
	while (std::getline(input_file, line))
	{
		for (int i = 0; i < line.length(); i++)
		{
			grid[row][i] = line[i];
		}
		row++;

		if (row == rows)
		{
			break;
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
	PerformMoves(move_chars);

	int64_t total_cost = 0;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			if (grid[i][j] == 'O')
			{
				int64_t cost = 100 * i + j;
				total_cost += cost;
			}
		}
	}

	std::cout << "Total Cost: " << total_cost << std::endl;
    return 0;
}
