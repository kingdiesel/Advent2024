// https://adventofcode.com/2024/day/12
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
constexpr int rows = 140;
constexpr int columns = 140;
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

	bool operator==(const GridPoint& other) const
	{
		return row == other.row && column == other.column;
	}

	int row = 0;
	int column = 0;
};

class Region
{
public:
	Region(char region_id) : m_region_id(region_id) 
	{
	
	}

	// The area of a region is simply the number of garden plots the region contains.
	uint64_t GetArea() const
	{
		return static_cast<uint64_t>(m_points.size());
	}

	// The perimeter of a region is the number of sides of garden plots in the region 
	// that do not touch another garden plot in the same region.
	uint64_t GetPerimeter() const
	{
		uint64_t perimeter = 0;
		for (const GridPoint& point : m_points)
		{
			GridPoint points[4];
			points[0] = GridPoint(point.row - 1, point.column); // Up
			points[1] = GridPoint(point.row + 1, point.column); // Down
			points[2] = GridPoint(point.row, point.column - 1); // Left
			points[3] = GridPoint(point.row, point.column + 1); // Right
			for (const GridPoint& neighbor : points)
			{
				if (!neighbor.IsValid() || (neighbor.IsValid() && neighbor.GetValue() != m_region_id))
				{
					perimeter++;
				}
			}
		}
		return perimeter;
	}

	uint64_t GetPrice() const
	{
		// A region of R plants with price 12 * 18 = 216.
		std::cout << "A region of " << m_region_id << " plants with price " << 
			GetArea() << " * " << GetPerimeter() << " = " << GetArea() * GetPerimeter() << std::endl;
		return GetArea() * GetPerimeter();
	}

	bool Contains(const GridPoint& point) const
	{
		return std::find(m_points.begin(), m_points.end(), point) != m_points.end();
	}

	void FillRegion_Recursive(const GridPoint& point)
	{
		/*
			return if:
				1. Out of bounds
				2. Already have this point in the region
				3. The point is not part of the region (i.e., its value does not match the region_id)
		*/
		if (!point.IsValid() || Contains(point) || point.GetValue() != m_region_id)
		{
			return;
		}

		m_points.push_back(point);

		GridPoint points[4];
		points[0] = GridPoint(point.row - 1, point.column); // Up
		points[1] = GridPoint(point.row + 1, point.column); // Down
		points[2] = GridPoint(point.row, point.column - 1); // Left
		points[3] = GridPoint(point.row, point.column + 1); // Right
		for (const GridPoint& neighbor : points)
		{
			FillRegion_Recursive(neighbor);
		}
	}
private:
	std::vector<GridPoint> m_points;
	char m_region_id = '0';
};

class Map
{
public:
	bool Contains(const GridPoint& point) const
	{
		for (const Region& region : m_regions)
		{
			if (region.Contains(point))
			{
				return true;
			}
		}
		return false;
	}

	std::vector<Region>& GetRegions()
	{
		return m_regions;
	}

	uint64_t GetPrice() const
	{
		uint64_t price = 0;
		for (const Region& region : m_regions)
		{
			price += region.GetPrice();
		}
		return price;
	}

private:
	std::vector<Region> m_regions;
};

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

	Map map;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			GridPoint point(r, c);
			if (map.Contains(point))
			{
				continue;
			}

			char region_id = point.GetValue();
			Region region(region_id);
			region.FillRegion_Recursive(point);
			map.GetRegions().push_back(region);
		}
	}
	const uint64_t price = map.GetPrice();
	std::cout << "Total price of all regions: " << price << std::endl;
	return 0;
}
