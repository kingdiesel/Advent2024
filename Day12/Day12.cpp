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

	bool IsValid_RegionGrid(int row, int col) const
	{
		if (row < 0 || row >= m_region_grid_rows || col < 0 || col >= m_region_grid_columns)
		{
			return false; // Out of bounds
		}
		return true;
	}

	bool HasLeftEdge_RegionGrid(int row, int col) const
	{
		assert(IsValid_RegionGrid(row, col));

		if (m_region_grid[row][col] == 0)
		{
			return false; // No edge if the cell is empty
		}
		// I have a left edge if:
		//		1. I am in the first column of the region grid, or
		//		2. The cell to the left of me is empty
		if (col == 0)
		{
			return true; // First column, so definitely has a left edge
		}

		if (m_region_grid[row][col - 1] == 0)
		{
			return true; // The cell to the left is empty
		}

		return false;
	}

	bool HasRightEdge_RegionGrid(int row, int col) const
	{
		assert(IsValid_RegionGrid(row, col));
		if (m_region_grid[row][col] == 0)
		{
			return false; // No edge if the cell is empty
		}
		// I have a right edge if:
		//		1. I am in the last column of the region grid, or
		//		2. The cell to the right of me is empty
		if (col == m_region_grid_columns - 1)
		{
			return true; // Last column, so definitely has a right edge
		}

		if (m_region_grid[row][col + 1] == 0)
		{
			return true; // The cell to the right is empty
		}

		return false;
	}

	bool HasTopEdge_RegionGrid(int row, int col) const
	{
		assert(IsValid_RegionGrid(row, col));
		if (m_region_grid[row][col] == 0)
		{
			return false; // No edge if the cell is empty
		}
		// I have a top edge if:
		//		1. I am in the first row of the region grid, or
		//		2. The cell above me is empty
		if (row == 0)
		{
			return true; // First row, so definitely has a top edge
		}

		if (m_region_grid[row - 1][col] == 0)
		{
			return true; // The cell above is empty
		}

		return false;
	}

	bool HasBottomEdge_RegionGrid(int row, int col) const
	{
		assert(IsValid_RegionGrid(row, col));
		if (m_region_grid[row][col] == 0)
		{
			return false; // No edge if the cell is empty
		}
		// I have a bottom edge if:
		//		1. I am in the last row of the region grid, or
		//		2. The cell below me is empty
		if (row == m_region_grid_rows - 1)
		{
			return true; // Last row, so definitely has a bottom edge
		}

		if (m_region_grid[row + 1][col] == 0)
		{
			return true; // The cell below is empty
		}

		return false;
	}

	uint64_t GetNumSides() const
	{
		int vertical_sides = 0;
		int horizontal_sides = 0;

		// count vertical sides
		for (int i = 0; i < m_region_grid_columns; ++i)
		{
			bool evaluating_left_edge = false;
			bool evaluating_right_edge = false;
			for (int j = 0; j < m_region_grid_rows; ++j)
			{
				if (HasLeftEdge_RegionGrid(j, i))
				{
					evaluating_left_edge = true;
				}
				else
				{
					if (evaluating_left_edge)
					{
						vertical_sides++;
					}
					evaluating_left_edge = false;
				}

				if (HasRightEdge_RegionGrid(j, i))
				{
					evaluating_right_edge = true;
				}
				else
				{
					if (evaluating_right_edge)
					{
						vertical_sides++;
					}
					evaluating_right_edge = false;
				}
			}
			if (evaluating_left_edge)
			{
				vertical_sides++;
			}
			if (evaluating_right_edge)
			{
				vertical_sides++;
			}
			evaluating_left_edge = false;
		}

		for (int i = 0; i < m_region_grid_rows; ++i)
		{
			bool evaluating_top_edge = false;
			bool evaluating_bottom_edge = false;
			for (int j = 0; j < m_region_grid_columns; ++j)
			{
				if (HasTopEdge_RegionGrid(i, j))
				{
					evaluating_top_edge = true;
				}
				else
				{
					if (evaluating_top_edge)
					{
						horizontal_sides++;
					}
					evaluating_top_edge = false;
				}

				if (HasBottomEdge_RegionGrid(i, j))
				{
					evaluating_bottom_edge = true;
				}
				else
				{
					if (evaluating_bottom_edge)
					{
						horizontal_sides++;
					}
					evaluating_bottom_edge = false;
				}
			}
			if (evaluating_top_edge)
			{
				horizontal_sides++;
			}
			if (evaluating_bottom_edge)
			{
				horizontal_sides++;
			}
			evaluating_top_edge = false;
		}
		
		return vertical_sides + horizontal_sides;
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
		//std::cout << "A region of " << m_region_id << " plants with price " << 
		//	GetArea() << " * " << GetPerimeter() << " = " << GetArea() * GetPerimeter() << std::endl;
		return GetArea() * GetPerimeter();
	}

	uint64_t GetBulkPrice() const
	{
		//std::cout << "A region of " << m_region_id << " plants with price " <<
		//	GetArea() << " * " << GetNumSides() << " = " << GetArea() * GetNumSides() << std::endl;
		return GetArea() * GetNumSides();
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

	void FillRegionGrid()
	{
		int row_min = 999, row_max = -999, col_min = 999, col_max = -999;
		for (const GridPoint& point : m_points)
		{
			row_min = std::min(row_min, point.row);
			row_max = std::max(row_max, point.row);
			col_min = std::min(col_min, point.column);
			col_max = std::max(col_max, point.column);
		}

		m_region_grid_rows = row_max - row_min + 1;
		m_region_grid_columns = col_max - col_min + 1;
		m_region_grid = new int*[m_region_grid_rows];
		for (int i = 0; i < m_region_grid_rows; ++i)
		{
			m_region_grid[i] = new int[m_region_grid_columns];
			std::fill(m_region_grid[i], m_region_grid[i] + m_region_grid_columns, 0);
		}

		for (const GridPoint& point : m_points)
		{
			int grid_row = point.row - row_min;
			int grid_column = point.column - col_min;
			m_region_grid[grid_row][grid_column] = 1;
		}
	}

	void PrintRegionGrid() const
	{
		std::cout << "Region " << m_region_id << " grid:" << std::endl;
		for (int i = 0; i < m_region_grid_rows; ++i)
		{
			for (int j = 0; j < m_region_grid_columns; ++j)
			{
				if (m_region_grid[i][j] == 0)
				{
					std::cout << '.'; // Empty space
				}
				else if (m_region_grid[i][j] == 1)
				{
					std::cout << m_region_id; // Region ID
				}
			}
			std::cout << std::endl;
		}
	}
private:
	std::vector<GridPoint> m_points;
	char m_region_id = '0';
	int** m_region_grid = nullptr;
	int m_region_grid_rows = 0;
	int m_region_grid_columns = 0;
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

	uint64_t GetBulkPrice() const
	{
		uint64_t price = 0;
		for (const Region& region : m_regions)
		{
			price += region.GetBulkPrice();
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
			region.FillRegionGrid();
			//region.PrintRegionGrid();
			map.GetRegions().push_back(region);
		}
	}
	//const uint64_t price = map.GetPrice();
	//std::cout << "Total price of all regions: " << price << std::endl;

	const uint64_t bulk_price = map.GetBulkPrice();
	std::cout << "Total bulk price of all regions: " << bulk_price << std::endl;
	return 0;
}
