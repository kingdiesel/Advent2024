// https://adventofcode.com/2024/day/11
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <sstream>
#include <map>
//#define SAMPLE_INPUT


int CountDigits(uint64_t n)
{
	return n == 0 ? 1 : static_cast<int>(std::floor(std::log10(std::abs(static_cast<double>(n)))) + 1);
}

class BigArray
{
public:
	BigArray()
	{
		for (uint64_t i = 0; i < max_chunks; ++i)
		{
			std::cout << "Creating chunk " << i + 1 << " of " << max_chunks << std::endl;
			chunks.push_back(new uint64_t[chunk_reserve]);
		}
	}
	
	const uint64_t& operator [](uint64_t index) const
	{
		assert(index < chunk_reserve * max_chunks);
		uint64_t chunk_index = index / chunk_reserve;
		uint64_t element_index = index % chunk_reserve;
		assert(chunk_index < chunks.size());
		return chunks[chunk_index][element_index];
	}

	uint64_t& operator [](uint64_t index)
	{
		assert(index < chunk_reserve * max_chunks);
		uint64_t chunk_index = index / chunk_reserve;
		uint64_t element_index = index % chunk_reserve;
		assert(chunk_index < chunks.size());
		return chunks[chunk_index][element_index];
	}

	void push_back(uint64_t value)
	{
		chunks[next_available_chunk][next_available_index] = value;
		next_available_index = next_available_index + 1 == chunk_reserve ? 0 : next_available_index + 1;
		next_available_chunk = next_available_index == 0 ? next_available_chunk + 1 : next_available_chunk;
		total_size++;
	}

	uint64_t size()
	{
		return total_size;
	}

	void clear()
	{
		total_size = 0;
		next_available_index = 0;
		next_available_chunk = 0;
	}

private:
	std::vector<uint64_t*> chunks;
	const uint64_t chunk_reserve = 536870912;
	const uint64_t max_chunks = 3;
	uint64_t total_size = 0;
	uint64_t next_available_chunk = 0;
	uint64_t next_available_index = 0;
};

void DoBlinks_StaticArrays(uint64_t* numbers_on, uint64_t* numbers_off, int& numbers_on_size, int& numbers_off_size)
{
	uint64_t powers_of_10[17] = { 0 };
	powers_of_10[2] = 10;
	powers_of_10[4] = 100;
	powers_of_10[6] = 1000;
	powers_of_10[8] = 10000;
	powers_of_10[10] = 100000;
	powers_of_10[12] = 1000000;
	powers_of_10[14] = 10000000;
	powers_of_10[16] = 100000000;
	int num_blinks = 75;
	for (int i = 0; i < num_blinks; ++i)
	{
		uint64_t* numbers = (i % 2 == 0) ? numbers_on : numbers_off;
		int& numbers_size = (i % 2 == 0) ? numbers_on_size : numbers_off_size;
		uint64_t* numbers_next = (i % 2 == 0) ? numbers_off : numbers_on;
		int& numbers_next_size = (i % 2 == 0) ? numbers_off_size : numbers_on_size;
		numbers_next_size = 0;
		for (size_t j = 0; j < numbers_size; ++j)
		{
			const uint64_t& n = numbers[j];
			int digits = CountDigits(n);
			if (n == 0)
			{
				numbers_next[numbers_next_size] = 1;
				numbers_next_size++;
			}
			else if (digits % 2 == 0)
			{
				uint64_t left_num = n / powers_of_10[digits];
				uint64_t right_num = n % powers_of_10[digits];
				numbers_next[numbers_next_size] = left_num;
				numbers_next_size++;
				numbers_next[numbers_next_size] = right_num;
				numbers_next_size++;
			}
			else
			{
				numbers_next[numbers_next_size] = n * 2024;
				numbers_next_size++;
			}
		}
		std::cout << "Blink " << i + 1 << ": " << numbers_next_size << " stones\n";
	}

	//for (uint64_t x : numbers)
	//{
	//	std::cout << x << ' ';
	//}
	//std::cout << '\n';

	std::cout << "Num stones: " << numbers_on_size << std::endl;
	std::cout << "Num stones: " << numbers_off_size << std::endl;
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
#ifdef STATIC_ARRAYS
	uint64_t* numbers_on = new uint64_t[536000000];
	uint64_t* numbers_off = new uint64_t[536000000];
	int numbers_on_size = 0;
	int numbers_off_size = 0;
#endif
	BigArray numbers_on, numbers_off;
	if (std::getline(input_file, line))
	{
		std::istringstream iss(line);
		uint64_t n;
		while (iss >> n)
		{
			numbers_on.push_back(n);
#ifdef STATIC_ARRAYS
			numbers_on[numbers_on_size] = n;
			numbers_on_size++;
#endif
		}
	}
#ifdef STATIC_ARRAYS
	DoBlinks_StaticArrays(numbers_on, numbers_off, numbers_on_size, numbers_off_size);
#endif 

	uint64_t powers_of_10[17] = { 0 };
	powers_of_10[2] = 10;
	powers_of_10[4] = 100;
	powers_of_10[6] = 1000;
	powers_of_10[8] = 10000;
	powers_of_10[10] = 100000;
	powers_of_10[12] = 1000000;
	powers_of_10[14] = 10000000;
	powers_of_10[16] = 100000000;
	int num_blinks = 50;
	for (int i = 0; i < num_blinks; ++i)
	{
		BigArray& numbers = (i % 2 == 0) ? numbers_on : numbers_off;
		BigArray& numbers_next = (i % 2 == 0) ? numbers_off : numbers_on;
		numbers_next.clear();
		for (uint64_t j = 0; j < numbers.size(); ++j)
		{
			const uint64_t& n = numbers[j];
			int digits = CountDigits(n);
			if (n == 0)
			{
				numbers_next.push_back(1);
			}
			else if (digits % 2 == 0)
			{
				uint64_t left_num = n / powers_of_10[digits];
				uint64_t right_num = n % powers_of_10[digits];
				numbers_next.push_back(left_num);
				numbers_next.push_back(right_num);
			}
			else
			{
				numbers_next.push_back(n * 2024);
			}
		}
		std::cout << "Blink " << i + 1 << ": " << numbers_next.size() << " stones" << std::endl;
	}

	std::cout << "Num stones: " << numbers_on.size() << std::endl;
	std::cout << "Num stones: " << numbers_off.size() << std::endl;
	return 0;
}
