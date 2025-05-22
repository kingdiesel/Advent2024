// https://adventofcode.com/2024/day/2
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>

bool IsSafe(std::vector<int>& list)
{
	// The levels are either all increasing or all decreasing.
	// Any two adjacent levels differ by at least one and at most three.
	if (list.size() == 1)
	{
		return true;
	}

	bool is_increasing = false;
	bool is_decreasing = false;
	for (int i = 0; i < list.size(); ++i)
	{
		if (i + 1 < list.size())
		{
			// if the difference to the next number is > 3, it's not safe
			if (std::abs(list[i + 1] - list[i]) > 3)
			{
				return false;
			}

			// if we're increasing
			if (list[i+1] - list[i] > 0)
			{
				// but we've already decreased, it's not safe
				if (is_decreasing)
				{
					return false;
				}

				// flag increasing
				is_increasing = true;
			}
			else if (list[i+1] - list[i] < 0) // if we're decreasing
			{
				// but we've already increased, it's not safe
				if (is_increasing)
				{
					return false;
				}

				// flag decreasing
				is_decreasing = true;
			}
			else
			{
				// if the difference is 0, it's not safe
				return false;
			}
		}
	}
	return true;
}

bool IsSafe2(std::vector<int>& list)
{
	for (int i = 0; i < list.size(); ++i)
	{
		std::vector<int> deep_list_copy = list;
		deep_list_copy.erase(deep_list_copy.begin() + i);
		if (IsSafe(deep_list_copy))
		{
			return true;
		}
	}
	return false;
}

int main()
{
	std::ifstream input_file("input.txt");
	if (!input_file)
	{
		std::cerr << "Cannot open file.";
		return 1;
	}

	std::vector<int> list;
	int num_safe_reports = 0;
	int num_safe_reports_part_2 = 0;

	std::regex re("(-?\\d+)");
	std::string line;
	while (std::getline(input_file, line))
	{
		auto begin = std::sregex_iterator(line.begin(), line.end(), re);
		auto end = std::sregex_iterator();

		for (auto it = begin; it != end; ++it) 
		{
			list.push_back(std::stoi(it->str()));
		}
		IsSafe(list) ? num_safe_reports++ : num_safe_reports;
		IsSafe2(list) ? num_safe_reports_part_2++ : num_safe_reports_part_2;
		list.clear();
	}

	std::cout << num_safe_reports << std::endl;
	std::cout << num_safe_reports_part_2 << std::endl;
}
