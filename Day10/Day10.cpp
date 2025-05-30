// https://adventofcode.com/2024/day/10
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

//#define SAMPLE_INPUT

#ifdef SAMPLE_INPUT
constexpr int rows = 8;
constexpr int columns = 8;
#else
constexpr int rows = 54;
constexpr int columns = 54;
#endif
constexpr int HEIGHT_MIN = 0;
constexpr int HEIGHT_MAX = 9;
static int grid[rows][columns] = {};
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
	int GetValue() const
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

void GetTrailScore(const GridPoint& point, const int current_height, std::vector<GridPoint>& reached)
{
	if (current_height == HEIGHT_MAX)
	{
		if (std::find(reached.begin(), reached.end(), point) == reached.end())
		{
			reached.push_back(point);
		}
		return;
	}

	std::vector<GridPoint> next_points;
	next_points.emplace_back(point.row - 1, point.column); // up
	next_points.emplace_back(point.row + 1, point.column); // down
	next_points.emplace_back(point.row, point.column - 1); // left
	next_points.emplace_back(point.row, point.column + 1); // right

	for (const GridPoint& next_point : next_points)
	{
		if (!next_point.IsValid())
		{
			continue;
		}

		const int next_height = next_point.GetValue();
		if (next_height == current_height + 1)
		{
			GetTrailScore(next_point, current_height + 1, reached);
		}
	}
}

int GetTrailRating(const GridPoint& point, const int current_height)
{
	if (current_height == HEIGHT_MAX)
	{
		return 1;
	}

	std::vector<GridPoint> next_points;
	next_points.emplace_back(point.row - 1, point.column); // up
	next_points.emplace_back(point.row + 1, point.column); // down
	next_points.emplace_back(point.row, point.column - 1); // left
	next_points.emplace_back(point.row, point.column + 1); // right

	int trail_rating = 0;
	for (const GridPoint& next_point : next_points)
	{
		if (!next_point.IsValid())
		{
			continue;
		}

		const int next_height = next_point.GetValue();
		if (next_height == current_height + 1)
		{
			trail_rating += GetTrailRating(next_point, current_height + 1);
		}
	}
	return trail_rating;
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
	std::vector<GridPoint> starting_points;
	while (std::getline(input_file, line))
	{
		for (int i = 0; i < line.length(); i++)
		{
			grid[row][i] = line[i] - '0';
			if (grid[row][i] == HEIGHT_MIN)
			{
				starting_points.emplace_back(row, i);
			}
		}
		row++;
	}

	int trail_scores = 0;
	std::vector<GridPoint> reached_points;
	for (const GridPoint& start_point : starting_points)
	{
		GetTrailScore(start_point, HEIGHT_MIN, reached_points);
		trail_scores += static_cast<int>(reached_points.size());
		reached_points.clear();
	}

	std::cout << "Trail Scores: " << trail_scores << std::endl;
	
	int trail_ratings = 0;
	for (const GridPoint& start_point : starting_points)
	{
		const int trail_raiting = GetTrailRating(start_point, HEIGHT_MIN);
		//std::cout << "Trail Rating for (" << start_point.row << ", " << start_point.column << "): " << trail_raiting << std::endl;
		trail_ratings += trail_raiting;
	}
	std::cout << "Trail Ratings: " << trail_ratings << std::endl;

    return 0;
}
