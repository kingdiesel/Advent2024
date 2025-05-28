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

#ifdef SAMPLE_INPUT
constexpr bool print = true;
#else
constexpr bool print = false;
#endif
struct FileBlock
{
	int file_id = 0;
	bool is_file = false;

	std::string ToString() const
	{
		return is_file ? std::to_string(file_id) : ".";
	}
};

struct FileDescriptor
{
	int starting_index = 0;
	int size = 0;
	int file_id = 0;
};

int largest_free_space_block = 0;

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
	}
	return checksum;
}

void RecomputeFreeSpaceMap(std::map<int, std::vector<int>>& free_space_to_index, std::vector<FileBlock>& file_blocks)
{
	free_space_to_index.clear();
	largest_free_space_block = 0;
	for (int i = 0; i < file_blocks.size(); ++i)
	{
		if (!file_blocks[i].is_file)
		{
			int free_start = i;
			for (int j = free_start; j < file_blocks.size(); ++j)
			{
				if (file_blocks[j].is_file)
				{
					int free_size = j - free_start;
					if (free_space_to_index.find(free_size) == free_space_to_index.end())
					{
						free_space_to_index[free_size] = std::vector<int>();
						largest_free_space_block = std::max(largest_free_space_block, free_size);
					}
					free_space_to_index[free_size].push_back(free_start);
					i = j - 1;
					break;
				}
			}
		}
	}
}



void Compress2(std::vector<FileBlock>& file_blocks, std::map<int, std::vector<int>>& free_space_to_index, std::vector<FileDescriptor> descriptors)
{
	// Process files from highest ID to lowest
	for (int idx = static_cast<int>(descriptors.size()) - 1; idx >= 0; --idx)
	{
		auto& desc = descriptors[idx];
		int old_start = desc.starting_index;
		int file_size = desc.size;

		// Find the leftmost free block that can fit this file
		int best_start = -1;
		int best_block_size = 0;
		for (auto const& [block_size, starts] : free_space_to_index)
		{
			if (block_size < file_size)
				continue;
			for (int start : starts)
			{
				if (start < old_start)
				{
					if (best_start == -1 || start < best_start)
					{
						best_start = start;
						best_block_size = block_size;
					}
				}
			}
		}

		// If no valid block was found, skip
		if (best_start == -1)
			continue;

		// Update descriptor to new location
		desc.starting_index = best_start;

		// Physically move the file blocks
		for (int j = 0; j < file_size; ++j)
		{
			file_blocks[best_start + j].file_id = desc.file_id;
			file_blocks[best_start + j].is_file = true;
		}

		// Clear old file blocks
		for (int j = old_start; j < old_start + file_size; ++j)
		{
			file_blocks[j].file_id = 0;
			file_blocks[j].is_file = false;
		}

		// Remove the used free block
		{
			auto& vec = free_space_to_index[best_block_size];
			vec.erase(std::find(vec.begin(), vec.end(), best_start));
			if (vec.empty())
				free_space_to_index.erase(best_block_size);
		}

		// Add leftover space from the block (if any)
		int leftover = best_block_size - file_size;
		if (leftover > 0)
		{
			free_space_to_index[leftover].push_back(best_start + file_size);
			auto& v = free_space_to_index[leftover];
			std::sort(v.begin(), v.end());
			largest_free_space_block = std::max(largest_free_space_block, leftover);
		}

		// Add the newly freed old space back into the map
		{
			free_space_to_index[file_size].push_back(old_start);
			auto& v = free_space_to_index[file_size];
			std::sort(v.begin(), v.end());
			largest_free_space_block = std::max(largest_free_space_block, file_size);
		}
	}
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
				digits.push_back(c - '0');
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
	if (print)
	{
		for (const auto& block : file_blocks)
		{
			std::cout << block.ToString();
		}

		std::cout << std::endl;
	}

	std::map<int, std::vector<int>> free_space_to_index;
	std::vector<FileDescriptor> file_descriptors;
	for (int i = 0; i < file_blocks.size(); ++i)
	{
		if (!file_blocks[i].is_file)
		{
			int free_start = i;
			for (int j = free_start; j < file_blocks.size(); ++j)
			{
				if (file_blocks[j].is_file)
				{
					int free_size = j - free_start;
					if (free_space_to_index.find(free_size) == free_space_to_index.end())
					{
						free_space_to_index[free_size] = std::vector<int>();
						largest_free_space_block = std::max(largest_free_space_block, free_size);
					}
					free_space_to_index[free_size].push_back(free_start);
					i = j-1;
					break;
				}
			}
		}
		else
		{
			int file_size = 0;
			int file_start = i;
			int current_file_id = file_blocks[i].file_id;
			while (i < file_blocks.size() && file_blocks[i].file_id == current_file_id)
			{
				file_size++;
				i++;
			}
			file_descriptors.emplace_back(FileDescriptor{ file_start, file_size, current_file_id });
			i--; // Adjust for the outer loop increment
		}
	}

	//Compress(file_blocks);
	Compress2(file_blocks, free_space_to_index, file_descriptors);
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
