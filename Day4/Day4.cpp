// https://adventofcode.com/2024/day/4
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
constexpr int rows = 140;
constexpr int columns = 140;

static char grid[rows][columns] = {};
struct GridPoint
{
	GridPoint(int in_row, int in_column)
		: row(in_row), column(in_column) {}


	bool IsValid() const
	{
		return row >= 0 && row < rows && column >= 0 && column < columns;
	}

	// call IsValid before using this function
	char GetLetter() const
	{
		assert(IsValid());
		return grid[row][column];
	}

	int row = 0;
	int column = 0;
};

// count number of XMAS words starting from this point going horizontal, vertical, diagonal, written backwards
int Count(const GridPoint& point)
{
	GridPoint vertical_up[3] = { GridPoint{point.row - 1, point.column }, GridPoint{ point.row - 2, point.column }, GridPoint{ point.row - 3, point.column } };
	GridPoint vertical_down[3] = { GridPoint{point.row + 1, point.column }, GridPoint{ point.row + 2, point.column }, GridPoint{ point.row + 3, point.column } };
	GridPoint horizontal_left[3] = { GridPoint{point.row, point.column - 1}, GridPoint{point.row, point.column - 2}, GridPoint{point.row, point.column - 3} };
	GridPoint horizontal_right[3] = { GridPoint{point.row, point.column + 1}, GridPoint{point.row, point.column + 2}, GridPoint{point.row, point.column + 3} };
	GridPoint diagonal_up_left[3] = { GridPoint{point.row - 1, point.column - 1}, GridPoint{point.row - 2, point.column - 2}, GridPoint{point.row - 3, point.column - 3} };
	GridPoint diagonal_up_right[3] = { GridPoint{point.row - 1, point.column + 1}, GridPoint{point.row - 2, point.column + 2}, GridPoint{point.row - 3, point.column + 3} };
	GridPoint diagonal_down_left[3] = { GridPoint{point.row + 1, point.column - 1}, GridPoint{point.row + 2, point.column - 2}, GridPoint{point.row + 3, point.column - 3} };
	GridPoint diagonal_down_right[3] = { GridPoint{point.row + 1, point.column + 1}, GridPoint{point.row + 2, point.column + 2}, GridPoint{point.row + 3, point.column + 3} };

	int count = 0;
	auto check = [&](const GridPoint* points)
	{
		if (points[0].IsValid() && points[1].IsValid() && points[2].IsValid())
		{
			if (points[0].GetLetter() == 'M' && points[1].GetLetter() == 'A' && points[2].GetLetter() == 'S')
			{
				count++;
			}
		}
	};

	check(vertical_up);
	check(vertical_down);
	check(horizontal_left);
	check(horizontal_right);
	check(diagonal_up_left);
	check(diagonal_up_right);
	check(diagonal_down_left);
	check(diagonal_down_right);

	return count;
}

/*
* point passed in is the center of the X
find two MAS in the shape of an X. One way to achieve that is like this:
M.S
.A.
M.S
*/

bool IsOpposite(GridPoint point1, GridPoint point2)
{
	return point1.GetLetter() == 'M' && point2.GetLetter() == 'S' ||
		point1.GetLetter() == 'S' && point2.GetLetter() == 'M';
}

int Count2(const GridPoint& point)
{
	GridPoint north_west{ point.row - 1, point.column - 1 };
	GridPoint north_east{ point.row - 1, point.column + 1 };
	GridPoint south_west{ point.row + 1, point.column - 1 };
	GridPoint south_east{ point.row + 1, point.column + 1 };

	// all four points must be valid to create the X
	if (north_west.IsValid() && north_east.IsValid() && south_west.IsValid() && south_east.IsValid())
	{
		if (IsOpposite(north_west, south_east) && IsOpposite(north_east, south_west))
		{
			return 1;
		}
	}
	return 0;
}

int main()
{
	std::ifstream input_file("input.txt");
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
	}

	int xmas_count = 0;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			GridPoint point(i, j);
			if (point.GetLetter() == 'X')
			{
				xmas_count += Count(point);
			}
		}
	}

	std::cout << xmas_count << std::endl;

	xmas_count = 0;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			GridPoint point(i, j);
			if (point.GetLetter() == 'A')
			{
				xmas_count += Count2(point);
			}
		}
	}

	std::cout << xmas_count << std::endl;
}
