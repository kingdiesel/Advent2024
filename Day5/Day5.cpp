// https://adventofcode.com/2024/day/5
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include <regex>
#include <map>
#include <sstream>

int main()
{
    std::map<int, std::vector<int>> page_rules;
	std::vector<std::vector<int>> pages_to_produce;
	std::vector<std::vector<int>> produced_pages;
    std::ifstream input("input.txt");
	std::regex re(R"(^(\d+)\|(\d+)$)");
	std::string line;
    while(std::getline(input, line))
	{
		// end of section 1
		if (line.empty())
		{
			break;
		}

		std::smatch m;

		if (std::regex_match(line, m, re))
		{
			int lhs = std::stoi(m[1].str());
			int rhs = std::stoi(m[2].str());
			if (page_rules.find(lhs) == page_rules.end())
			{
				page_rules[lhs] = std::vector<int>();
			}
			page_rules[lhs].push_back(rhs);
		}
	}

	// read in section 2
	while (std::getline(input, line))
	{
		std::vector<int> nums;
		std::stringstream ss(line);
		std::string token;

		while (std::getline(ss, token, ','))
		{
			nums.push_back(std::stoi(token));
		}

		pages_to_produce.push_back(nums);
	}

	// loop over pages to produce
	for(const auto& pages : pages_to_produce)
	{
		for (int page_num : pages)
		{
			assert(page_rules.find(page_num) != page_rules.end());
		}
	}
}
