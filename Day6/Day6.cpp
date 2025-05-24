// https://adventofcode.com/2024/day/6
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
//#define SAMPLE_INPUT

#ifdef SAMPLE_INPUT
constexpr int rows = 10;
constexpr int columns = 10;
#else
constexpr int rows = 130;
constexpr int columns = 130;
#endif

static char grid[rows][columns] = {};
constexpr char EMPTY_CELL = '.';
constexpr char OBSTRUCTION_CELL = '#';
constexpr char GUARD_CELL = '^';
constexpr int MAX_STEPS = rows * columns;
bool IsBlocked(struct GridPoint& next_location);

enum Directions
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
Directions current_direction = Directions::UP;

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

	int row = 0;
	int column = 0;
};
GridPoint current_location;
GridPoint FindStartLocation()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (grid[i][j] == GUARD_CELL)
			{
				return GridPoint(i, j);
			}
		}
	}
	return GridPoint(-1, -1); // Not found
}

void TurnRight()
{
	switch (current_direction)
	{
	case Directions::UP:
		current_direction = Directions::RIGHT;
		break;
	case Directions::RIGHT:
		current_direction = Directions::DOWN;
		break;
	case Directions::DOWN:
		current_direction = Directions::LEFT;
		break;
	case Directions::LEFT:
		current_direction = Directions::UP;
		break;
	}
}

void GetObstructionList(std::vector<GridPoint>& obstructions)
{
	GridPoint start_location = FindStartLocation();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			// if an obstruction already exists
			if (grid[i][j] == OBSTRUCTION_CELL || grid[i][j] == GUARD_CELL)
			{
				continue;
			}

			char cached_cell_value = grid[i][j];
			grid[i][j] = OBSTRUCTION_CELL;
			current_location = start_location;
			GridPoint next_location;
			current_direction = Directions::UP;
			bool looped = false;
			int num_steps_taken = 0;
			while (next_location.IsValid())
			{
				if (!IsBlocked(next_location))
				{
					current_location = next_location;
					if (next_location.IsValid())
					{
						num_steps_taken++;
						if (num_steps_taken > MAX_STEPS)
						{
							looped = true; // looped around the grid
							break;
						}
					}
				}
				else
				{
					TurnRight();
				}
			}

			grid[i][j] = cached_cell_value; // restore the cell value
			if (looped)
			{
				obstructions.push_back(GridPoint(i, j));
			}
		}
	}
}

bool IsBlocked(GridPoint& next_location)
{
	// Check if the next cell in the current direction is blocked
	next_location = current_location;
	switch (current_direction)
	{
	case Directions::UP:
		next_location.row -= 1;
		break;
	case Directions::DOWN:
		next_location.row += 1;
		break;
	case Directions::LEFT:
		next_location.column -= 1;
		break;
	case Directions::RIGHT:
		next_location.column += 1;
		break;
	}

	if (!next_location.IsValid())
	{
		// found exit
		return false;
	}

	if (next_location.GetLetter() == '#')
	{
		return true;
	}

	return false;
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

	current_location = FindStartLocation();
	assert(current_location.IsValid() && "Start location not found.");
	GridPoint next_location;
	std::vector<GridPoint> unique_locations;
	unique_locations.push_back(current_location);
	while (next_location.IsValid())
	{
		if (!IsBlocked(next_location))
		{
			current_location = next_location;
			if (next_location.IsValid())
			{
				if (std::find(unique_locations.begin(), unique_locations.end(), current_location) == unique_locations.end())
				{
					unique_locations.push_back(current_location);
				}
			}
		}
		else
		{
			TurnRight();
		}
	}

	std::cout << "Result: " << unique_locations.size() << std::endl;

	std::vector<GridPoint> obstructions;
	GetObstructionList(obstructions);

	std::cout << "Result: " << obstructions.size() << std::endl;
	return 0;
}
