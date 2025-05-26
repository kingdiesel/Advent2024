// https://adventofcode.com/2024/day/8
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cassert>
//#define SAMPLE_INPUT

#ifdef SAMPLE_INPUT
constexpr int rows = 12;
constexpr int columns = 12;
#else
constexpr int rows = 50;
constexpr int columns = 50;
#endif
constexpr char EMPTY_CELL = '.';
constexpr char ANTI_NODE_CELL = '#';
static char grid[rows][columns] = { EMPTY_CELL };
static char antinode_grid[rows][columns] = { EMPTY_CELL };

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
	char GetLetter() const
	{
		assert(IsValid());
		return grid[row][column];
	}

	bool operator==(const GridPoint& other) const
	{
		return row == other.row && column == other.column;
	}

	bool operator!=(const GridPoint& other) const
	{
		return !(*this == other);
	}

	GridPoint operator-(const GridPoint& other) const
	{
		return GridPoint(row - other.row, column - other.column);
	}

	GridPoint operator+(const GridPoint& other) const
	{
		return GridPoint(row + other.row, column + other.column);
	}

	GridPoint operator-=(const GridPoint& other)
	{
		row -= other.row;
		column -= other.column;
		return *this;
	}

	GridPoint operator +=(const GridPoint& other)
	{
		row += other.row;
		column += other.column;
		return *this;
	}

	int row = 0;
	int column = 0;
};

// get grid points that are colinear with the start and end points that are the same distance from each point and twice the distance
// from the other point
void GetColinearGridPoints(const GridPoint& start, const GridPoint& end, std::vector<GridPoint>& out_points)
{
	assert(start.IsValid() && end.IsValid());
	assert(out_points.size() == 0);

	GridPoint start_to_end = end - start;
	GridPoint colinear_one = end + start_to_end;

	GridPoint end_to_start = start - end;
	GridPoint colinear_two = start + end_to_start;

	out_points.push_back(colinear_one);
	out_points.push_back(colinear_two);
}

int GetAntinodeCount()
{
	int count = 0;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			if (antinode_grid[i][j] == ANTI_NODE_CELL)
			{
				count++;
			}
		}
	}
	return count;
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
	}

	// frequency to list of grid points
	std::map<char, std::vector<GridPoint>> antenna_map;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			if (grid[i][j] != EMPTY_CELL)
			{
				char frequency = grid[i][j];
				antenna_map[frequency].emplace_back(i, j);
			}
		}
	}

	for(const auto& [frequency, points] : antenna_map)
	{
		if (points.size() < 2)
		{
			continue; // need at least two points to form an antinode
		}

		for (size_t i = 0; i < points.size(); ++i)
		{
			for (size_t j = 0; j < points.size(); ++j)
			{
				if (i == j)
				{
					continue;
				}

				GridPoint start = points[i];
				GridPoint end = points[j];

				std::vector<GridPoint> colinear_points;
				GetColinearGridPoints(start, end, colinear_points);

				for (const GridPoint& point : colinear_points)
				{
					if (point.IsValid())
					{
						antinode_grid[point.row][point.column] = ANTI_NODE_CELL;
					}
				}
			}
		}
	}

	std::cout << "Result:" << GetAntinodeCount() << std::endl;
	return 0;
}