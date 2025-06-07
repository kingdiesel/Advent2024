// https://adventofcode.com/2024/day/16
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
constexpr int rows = 17;
constexpr int columns = 17;
#else
constexpr int rows = 141;
constexpr int columns = 141;
#endif
static char grid[rows][columns] = {};
enum Directions
{
	NORTH = 0,
	SOUTH = 1,
	EAST = 2,
	WEST = 3
};
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

GridPoint reindeer_location;
Directions reindeer_direction = EAST;
std::vector<int> scores;
std::vector<GridPoint> current_path;

Directions GetDirectionBetweenPoints(const GridPoint& from, const GridPoint& to)
{
	if (to.row < from.row)
		return NORTH;
	else if (to.row > from.row)
		return SOUTH;
	else if (to.column < from.column)
		return WEST;
	else if (to.column > from.column)
		return EAST;
	return NORTH; // default case, should not happen
}

char GetDirectionChar(Directions direction)
{
	switch (direction)
	{
	case NORTH: return '^';
	case SOUTH: return 'v';
	case EAST: return '>';
	case WEST: return '<';
	default: return '?'; // should not happen
	}
}

void PrintPath()
{
	char scratch_grid[rows][columns] = {'0'};
	memcpy(scratch_grid, grid, sizeof(grid));
	for (int i = 0; i < current_path.size(); ++i)
	{
		const GridPoint& point = current_path[i];
		if (point.IsValid() && i > 0)
		{
			scratch_grid[point.row][point.column] = GetDirectionChar(GetDirectionBetweenPoints(current_path[i - 1], point));
		}
	}

	// output
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			std::cout << scratch_grid[i][j];
		}
		std::cout << std::endl;
	}
}

void FindExits(int& current_score, GridPoint current_point)
{
	if (current_point.GetValue() == 'E')
	{
		scores.push_back(current_score);
		//std::cout << current_score << std::endl;
		//PrintPath();
		return;
	}
	current_path.push_back(current_point);
	GridPoint north(current_point.row - 1, current_point.column);
	GridPoint south(current_point.row + 1, current_point.column);
	GridPoint east(current_point.row, current_point.column + 1);
	GridPoint west(current_point.row, current_point.column - 1);
	GridPoint directions[] = { north, south, east, west };
	bool visited[] = { false, false, false, false };
	visited[NORTH] = std::find(current_path.begin(), current_path.end(), directions[NORTH]) != current_path.end();
	visited[SOUTH] = std::find(current_path.begin(), current_path.end(), directions[SOUTH]) != current_path.end();
	visited[EAST] = std::find(current_path.begin(), current_path.end(), directions[EAST]) != current_path.end();
	visited[WEST] = std::find(current_path.begin(), current_path.end(), directions[WEST]) != current_path.end();

	for (int i = NORTH; i <= WEST; ++i)
	{
		if (visited[i])
		{
			continue;
		}
		if (directions[i].IsValid() && (directions[i].GetValue() == '.' || directions[i].GetValue() == 'E'))
		{
			current_score += i == reindeer_direction ? 1 : 1001;
			Directions previous_direction = reindeer_direction;
			reindeer_direction = static_cast<Directions>(i);
			FindExits(current_score, directions[i]);
			reindeer_direction = previous_direction;
			current_score -= i == reindeer_direction ? 1 : 1001;
		}
	}

	current_path.pop_back();
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

	// find the reindeer starting location
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (grid[i][j] == 'S')
			{
				reindeer_location.row = i;
				reindeer_location.column = j;
				reindeer_location.SetValue('.');
				break;
			}
		}
	}

	int current_score = 0;
	FindExits(current_score, reindeer_location);

	std::sort(scores.begin(), scores.end());
	std::cout << "Part 1: " << scores.front() << std::endl;

    return 0;
}
