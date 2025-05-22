// https://adventofcode.com/2024/day/1
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>

int CountOccurences(std::vector<int>& list, int num)
{
	int occurences = 0;
	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i] == num)
		{
			occurences++;
		}
	}
	return occurences;
}

int main()
{
	std::ifstream input_file("input.txt");
	if (!input_file)
	{
		std::cerr << "Cannot open file.";
		return 1;
	}

	int left_list_number, right_list_number;
	std::vector<int> left_list, right_list;
	while (input_file >> left_list_number >> right_list_number)
	{
		left_list.push_back(left_list_number);
		right_list.push_back(right_list_number);
	}

	assert(left_list.size() == right_list.size());

	std::sort(left_list.begin(), left_list.end());
	std::sort(right_list.begin(), right_list.end());

	int difference_amount = 0;
	for (int i = 0; i < left_list.size(); ++i)
	{
		difference_amount += std::abs(left_list[i] - right_list[i]);
	}

	std::cout << "part 1: " << difference_amount << std::endl;

	int similarity_score = 0;
	for (int i = 0; i < left_list.size(); ++i)
	{
		similarity_score += left_list[i] * CountOccurences(right_list, left_list[i]);
	}

	std::cout << "part 1: " << similarity_score << std::endl;
}
