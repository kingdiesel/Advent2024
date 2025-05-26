// https://adventofcode.com/2024/day/9
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cassert>
#include <cstdlib>
//#define SAMPLE_INPUT

struct FileBlock
{
	int file_id = 0;
	bool is_file = false;

	std::string ToString() const
	{
		return is_file ? std::to_string(file_id) : ".";
	}
};

void Compress(std::vector<FileBlock>& file_blocks)
{
	int reverse_index = static_cast<int>(file_blocks.size()) - 1;
	for (int i = 0; i < file_blocks.size(); ++i)
	{
		if (!file_blocks[i].is_file)
		{
			while (!file_blocks[reverse_index].is_file && reverse_index > i)
			{
				reverse_index--;
			}
			std::swap(file_blocks[i], file_blocks[reverse_index]);
		}
	}
}

int64_t GetChecksum(std::vector<FileBlock>& file_blocks)
{
	int64_t checksum = 0;
	for (int i = 0; i < file_blocks.size(); ++i)
	{
		if (file_blocks[i].is_file)
		{
			checksum += static_cast<int64_t>(i) * static_cast<int64_t>(file_blocks[i].file_id);
		}
		else
		{
			break;
		}
	}
	return checksum;
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
	std::vector<int> digits;
	if (std::getline(input_file, line))
	{
		for (char c : line)
		{
			if (std::isdigit(static_cast<unsigned char>(c)))
			{
				digits.push_back(atoi(&c));
			}
		}
	}

	std::vector<FileBlock> file_blocks;
	int id = 0;
	for (int i = 0; i < digits.size(); ++i)
	{
		const bool is_file = i % 2 == 0;
		for (int j = 0; j < digits[i]; ++j)
		{
			
			file_blocks.emplace_back(FileBlock{ id, is_file });
		}
		if (is_file)
		{
			id++;
		}
	}
	constexpr bool print = false;
	if (print)
	{
		for (const auto& block : file_blocks)
		{
			std::cout << block.ToString();
		}

		std::cout << std::endl;
	}

	Compress(file_blocks);

	if (print)
	{
		for (const auto& block : file_blocks)
		{
			std::cout << block.ToString();
		}
		std::cout << std::endl;
	}

	std::cout << "Checksum: " << GetChecksum(file_blocks) << std::endl;

	return 0;
}
