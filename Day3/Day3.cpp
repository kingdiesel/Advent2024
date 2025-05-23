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
	auto it = std::sregex_iterator(input_as_line.begin(), input_as_line.end(), re);
	auto end = std::sregex_iterator();

	int sum = 0;
	for (; it != end; ++it)
	{
		int a = std::stoi((*it)[1].str());
		int b = std::stoi((*it)[2].str());

		sum += a * b;
	}

	std::cout << sum << std::endl;

	std::regex re2(R"(do\(\).*?don't\(\))");
	std::vector<std::string> groups;

	for (auto it = std::sregex_iterator(input_as_line.begin(), input_as_line.end(), re2); it != std::sregex_iterator(); ++it)
	{
		groups.push_back(it->str());
	}

	int sum2 = 0;
	for (std::size_t i = 0; i < groups.size(); ++i)
	{
		auto it = std::sregex_iterator(groups[i].begin(), groups[i].end(), re);
		auto end = std::sregex_iterator();

		for (; it != end; ++it)
		{
			int a = std::stoi((*it)[1].str());
			int b = std::stoi((*it)[2].str());
			sum2 += a * b;
		}
	}

	std::cout << sum2 << std::endl;
}
