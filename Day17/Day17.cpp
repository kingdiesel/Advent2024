// https://adventofcode.com/2024/day/17
#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <map>
#include <regex>
#include <chrono>
#include "CPU.h"
//#define SAMPLE_INPUT

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
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
    int registerA = 0;
    int registerB = 0;
    int registerC = 0;
    std::vector<int> program;

    for (int i = 0; i < 3; ++i)
    {
        if (!std::getline(input_file, line))
        {
            std::cerr << "Unexpected end of input reading registers" << std::endl;
            return 1;
        }

        auto colon = line.find(':');
        if (colon == std::string::npos)
        {
            continue;
        }

        std::string name = line.substr(0, colon);
        int value = std::stoi(line.substr(colon + 1));

        if (name.find('A') != std::string::npos)
        {
            registerA = value;
        }
        else if (name.find('B') != std::string::npos)
        {
            registerB = value;
        }
        else if (name.find('C') != std::string::npos)
        {
            registerC = value;
        }
    }

    std::getline(input_file, line);

    if (!std::getline(input_file, line))
    {
        std::cerr << "Unexpected end of input reading program" << std::endl;
        return 1;
    }

    auto colon = line.find(':');

    if (colon == std::string::npos)
    {
        std::cerr << "Invalid program line format" << std::endl;
        return 1;
    }

    std::string nums = line.substr(colon + 1);
    std::istringstream iss(nums);
    int instr;
    char comma;

    while (iss >> instr)
    {
        program.push_back(instr);
        iss >> comma;
    }

    CPU cpu;
    cpu.SetRegisterA(registerA);
    cpu.SetRegisterB(registerB);
    cpu.SetRegisterC(registerC);
    cpu.LoadProgram(program);

    auto HaltCallback = [&]() 
    {
        std::cout << std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
        exit(0);
    };
    cpu.SetHaltCallback(HaltCallback);
    cpu.RunProgram();
    assert(false);
    return 0;
}
