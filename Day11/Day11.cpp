// https://adventofcode.com/2024/day/11
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <sstream>

//#define SAMPLE_INPUT

int CountDigits(uint64_t n)
{
	return n == 0 ? 1 : static_cast<int>(std::floor(std::log10(std::abs(static_cast<double>(n)))) + 1);
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
	std::vector<uint64_t> numbers;
	if (std::getline(input_file, line))
	{
		std::istringstream iss(line);
		uint64_t n;
		while (iss >> n)
		{
			numbers.push_back(n);
		}
	}

	int num_blinks = 25;

	for (int i = 0; i < num_blinks; ++i)
	{
		for (size_t j = 0; j < numbers.size(); ++j)
		{
			uint64_t& n = numbers[j];
			if (n == 0)
			{
				n = 1;
			}
			else if (CountDigits(n) % 2 == 0)
			{
				const std::string str = std::to_string(n);
				const std::string left_half = str.substr(0, str.size() / 2);
				const std::string right_half = str.substr(str.size() / 2);
				uint64_t left_num = std::stoull(left_half);
				uint64_t right_num = std::stoull(right_half);
				n = left_num;
				if (j == numbers.size() - 1)
				{
					numbers.push_back(right_num);
					break;
				}
				else
				{
					numbers.insert(numbers.begin() + j + 1, right_num);
					j++;
				}
			}
			else
			{
				n *= 2024;
			}
		}
	}

	//for (uint64_t x : numbers)
	//{
	//	std::cout << x << ' ';
	//}
	//std::cout << '\n';

	std::cout << "Num stones: " << numbers.size() << std::endl;
    return 0;
}
