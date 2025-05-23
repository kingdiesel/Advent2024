// https://adventofcode.com/2024/day/3
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include <regex>

int main()
{
	std::ifstream input_file("input.txt");
	if (!input_file)
	{
		std::cerr << "Cannot open file.";
		return 1;
	}

	std::string line;
	std::string input_as_line;
	while (std::getline(input_file, line))
	{
		input_as_line += line;
	}

	std::regex re(R"(mul\((\d+),(\d+)\))");
	std::vector<std::pair<int, int>> mul_pairs;

	auto it = std::sregex_iterator(input_as_line.begin(), input_as_line.end(), re);
	auto end = std::sregex_iterator();

	int sum = 0;
	for (; it != end; ++it)
	{
		int a = std::stoi((*it)[1].str());
		int b = std::stoi((*it)[2].str());
		mul_pairs.emplace_back(a, b);

		sum += a * b;
	}

	std::cout << sum << std::endl;
}
