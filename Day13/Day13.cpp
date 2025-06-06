// https://adventofcode.com/2024/day/13
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <map>
#include <regex>
//#define SAMPLE_INPUT

constexpr int a_button_cost = 3;
constexpr int b_button_cost = 1;

struct Point
{
    Point() = default;

    int64_t x = 0;
    int64_t y = 0;
};

struct ClawMachine
{
    Point A;
    Point B;
    Point prize_location;
};

//int64_t GetClawMachineCost(const ClawMachine& cm)
//{
//    int64_t cost = std::numeric_limits<int64_t>::max();
//    bool found_combination = false;
//    int64_t a_presses = 0;
//    int64_t b_presses = 0;
//
//    for (a_presses = 0; a_presses <= 100; ++a_presses)
//	{
//		for (b_presses = 0; b_presses <= 100; ++b_presses)
//		{
//			// Calculate the final position after pressing buttons
//            int64_t final_x = cm.A.x * a_presses + cm.B.x * b_presses;
//            int64_t final_y = cm.A.y * a_presses + cm.B.y * b_presses;
//
//			// Check if this position matches the prize location
//			if (final_x == cm.prize_location.x && final_y == cm.prize_location.y)
//			{
//				found_combination = true;
//                int64_t current_cost = a_button_cost * a_presses + b_button_cost * b_presses;
//				cost = std::min(cost, current_cost);
//			}
//		}
//	}
//
//	return found_combination ? cost : -1;
//}

int64_t GetClawMachineCost(const ClawMachine& m)
{
    // Compute the determinant
    int64_t det = m.A.x * m.B.y - m.A.y * m.B.x;
    if (det == 0)
    {
        return 0;
    }

    constexpr int64_t OFFSET = 10000000000000;
    int64_t Tx = m.prize_location.x + OFFSET;
    int64_t Ty = m.prize_location.y + OFFSET;

    // Cramer's rule numerators
    int64_t numA = Tx * m.B.y - Ty * m.B.x;
    int64_t numB = m.A.x * Ty - m.A.y * Tx;

    // Check exact divisibility
    if (numA % det != 0 || numB % det != 0)
    {
        return 0;
    }

    int64_t a = numA / det;
    int64_t b = numB / det;
    if (a < 0 || b < 0)
    {
        return 0;
    }

    return a_button_cost * a + b_button_cost * b;
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

    std::vector<ClawMachine> machines;
    std::string line;

    std::regex button_regex(R"(^Button\s+([AB]):\s*X\+(\d+),\s*Y\+(\d+)\s*$)");
    std::regex prize_regex(R"(^Prize:\s*X=(\d+),\s*Y=(\d+)\s*$)");


    while (std::getline(input_file, line))
    {
        if (line.empty())
            continue;

        std::smatch match;
        ClawMachine cm;

        // Parse Button A line
        if (std::regex_match(line, match, button_regex))
        {
            // match[1] = "A" or "B", match[2] = X value, match[3] = Y value
            char which = match[1].str()[0];
            int xval = std::stoi(match[2].str());
            int yval = std::stoi(match[3].str());
            if (which == 'A')
            {
                cm.A.x = xval;
                cm.A.y = yval;
            }
            else
            {
                cm.B.x = xval;
                cm.B.y = yval;
            }
        }
        else
        {
            std::cerr << "Unexpected line (expected Button): " << line << "\n";
            return 1;
        }

        // Parse Button B line
        if (!std::getline(input_file, line))
        {
            std::cerr << "Unexpected EOF reading Button B.\n";
            return 1;
        }
        if (std::regex_match(line, match, button_regex))
        {
            char which = match[1].str()[0];
            int xval = std::stoi(match[2].str());
            int yval = std::stoi(match[3].str());
            if (which == 'A')
            {
                cm.A.x = xval;
                cm.A.y = yval;
            }
            else
            {
                cm.B.x = xval;
                cm.B.y = yval;
            }
        }
        else
        {
            std::cerr << "Unexpected line (expected Button): " << line << "\n";
            return 1;
        }

        // Parse Prize line
        if (!std::getline(input_file, line))
        {
            std::cerr << "Unexpected EOF reading Prize.\n";
            return 1;
        }
        if (std::regex_match(line, match, prize_regex))
        {
            int xval = std::stoi(match[1].str());
            int yval = std::stoi(match[2].str());
            cm.prize_location.x = xval;
            cm.prize_location.y = yval;
        }
        else
        {
            std::cerr << "Unexpected line (expected Prize): " << line << "\n";
            return 1;
        }

        machines.push_back(cm);
    }

    int64_t total_cost = 0;
    for (const auto& machine : machines)
	{
        const int64_t cost = GetClawMachineCost(machine);
        if (cost != -1)
        {
            total_cost += cost;
        }
	}

    std::cout << "Total cost: " << total_cost << std::endl;

    return 0;
}
