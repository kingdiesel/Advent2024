// https://adventofcode.com/2024/day/5
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include <regex>
#include <map>
#include <random>
#include <sstream>

bool FollowsRule(const std::vector<int>& pages, std::map<int, std::vector<int>>& page_rules, std::pair<int, int>& broken_rule)
{
	bool rule_broken = false;
	for (int i = 0; i < pages.size() && !rule_broken; ++i)
	{
		int page_num = pages[i];
		if (page_rules.find(page_num) == page_rules.end())
		{
			continue;
		}
		// get rules for this number
		std::vector<int>& rules = page_rules[page_num];

		// the numbers appearing in the rules indicate that our current page (page_num)
		// must appear before the pages in rules
		for (int rule : rules)
		{
			// for each number
			for (int j = 0; j < pages.size() && !rule_broken; ++j)
			{
				// skip the current page, there are no self rules
				if (j == i)
				{
					continue;
				}

				if (std::find(rules.begin(), rules.end(), pages[j]) != rules.end())
				{
					if (j < i)
					{
						//std::cout << "List [";
						//for (int i = 0; i < pages.size(); ++i)
						//{
						//	std::cout << pages[i];
						//	if (i < pages.size() - 1)
						//	{
						//		std::cout << ",";
						//	}
						//}
						//std::cout << "] breaks rule " << page_num << "|" << *std::find(rules.begin(), rules.end(), pages[j]) << std::endl;
						broken_rule.first = page_num;
						broken_rule.second = *std::find(rules.begin(), rules.end(), pages[j]);
						rule_broken = true;
					}
				}
			}
		}
	}
	return !rule_broken;
}

void FixList(std::vector<int>& pages, std::map<int, std::vector<int>>& page_rules)
{
	std::pair<int, int> broken_rule;
	// brute force, keep swapping rule breaks until we get success
	while (!FollowsRule(pages, page_rules, broken_rule))
	{
		// locate index of broken_rule.first
		auto it = std::find(pages.begin(), pages.end(), broken_rule.first);
		// locate the index of broken_rule.second
		auto it2 = std::find(pages.begin(), pages.end(), broken_rule.second);
		std::swap(*it, *it2);
	}
}

int main()
{
    std::map<int, std::vector<int>> page_rules;
	std::vector<std::vector<int>> pages_to_produce;
	std::vector<std::vector<int>> produced_pages;
	std::vector<std::vector<int>> good_lists;
	std::vector<std::vector<int>> bad_lists;
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
		std::pair<int, int> broken_rule;
		const bool follows_rules = FollowsRule(pages, page_rules, broken_rule);
		if (follows_rules)
		{
			good_lists.push_back(pages);
		}
		else
		{
			bad_lists.push_back(pages);
		}
	}

	int result = 0;
	// for each of the good lists, find the number in the middle index of the list
	for (const auto& good_list : good_lists) 
	{
		int middle_index = static_cast<int>(good_list.size()) / 2;
		result += good_list[middle_index];
	}

	std::cout << "Result: " << result << std::endl;

	for (auto& bad_list : bad_lists)
	{
		FixList(bad_list, page_rules);
	}

	result = 0;
	for (const auto& bad_list : bad_lists)
	{
		int middle_index = static_cast<int>(bad_list.size()) / 2;
		result += bad_list[middle_index];
	}
	std::cout << "Result: " << result << std::endl;
	return 0;
}
