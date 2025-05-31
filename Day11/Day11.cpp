// https://adventofcode.com/2024/day/11
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <sstream>
#include <unordered_map>
//#define SAMPLE_INPUT

struct ValueIterationsKey
{
	ValueIterationsKey(uint64_t v, uint32_t i) : value(v), iterations(i) {}
	uint64_t value;
	uint32_t iterations;

	bool operator==(const ValueIterationsKey& other) const
	{
		return value == other.value && iterations == other.iterations;
	}
};

struct ValueIterationsKeyHash
{
	size_t operator()(const ValueIterationsKey& Key)  const
	{
		return std::hash<uint64_t>()(Key.value) ^ (std::hash<uint32_t>()(Key.iterations) << 1);
	}
};

std::unordered_map<ValueIterationsKey, uint64_t, ValueIterationsKeyHash> value_iterations_map;
uint64_t powers_of_10[17] = { 0 };

int CountDigits(uint64_t n)
{
	return n == 0 ? 1 : static_cast<int>(std::floor(std::log10(std::abs(static_cast<double>(n)))) + 1);
}

uint64_t GetValueIterations(uint64_t value, uint32_t iterations)
{
	ValueIterationsKey key{ value, iterations };
	auto it = value_iterations_map.find(key);
	if (it != value_iterations_map.end())
	{
		return it->second;
	}

	if (iterations == 0)
	{
		value_iterations_map[key] = 1;
		return 1;
	}

	uint64_t result = value;
	int digits = CountDigits(result);
	if (result == 0)
	{
		result = GetValueIterations(1, iterations - 1);
	}
	else if (digits % 2 == 0)
	{
		uint64_t left_num = result / powers_of_10[digits];
		uint64_t right_num = result % powers_of_10[digits];
		result = GetValueIterations(left_num, iterations - 1) + GetValueIterations(right_num, iterations - 1);
	}
	else
	{
		result = GetValueIterations(result * 2024, iterations - 1);
	}
	value_iterations_map[key] = result;
	return result;
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

	powers_of_10[2] = 10;
	powers_of_10[4] = 100;
	powers_of_10[6] = 1000;
	powers_of_10[8] = 10000;
	powers_of_10[10] = 100000;
	powers_of_10[12] = 1000000;
	powers_of_10[14] = 10000000;
	powers_of_10[16] = 100000000;
	int num_blinks = 75;
	uint64_t total_stones = 0;
	for (int i = 0; i < numbers.size(); ++i)
	{
		total_stones += GetValueIterations(numbers[i], num_blinks);
	}
	std::cout << "Num stones: " << total_stones << std::endl;
	return 0;
}
