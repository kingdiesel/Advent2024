// https://adventofcode.com/2024/day/14
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <map>
#include <regex>
//#define SAMPLE_INPUT
#ifdef SAMPLE_INPUT
constexpr int max_y = 7;
constexpr int max_x = 11;
#else
constexpr int max_y = 103;
constexpr int max_x = 101;
#endif
constexpr int y_middle = (max_y - 1) / 2;
constexpr int x_middle = (max_x - 1) / 2;

enum Quadrant
{
    TOP_LEFT = 0,
    TOP_RIGHT = 1,
    BOTTOM_LEFT = 2,
    BOTTOM_RIGHT = 3,
    NONE = 4
};

struct Point
{
    Point() = default;
    Point(int64_t x, int64_t y) 
		: x(x), y(y) 
	{ 
	}

    Quadrant GetQuadrant() const
    {
        if (y == y_middle || x == x_middle)
        {
            return NONE;
        }
        if (x < (max_x / 2) && y < (max_y / 2))
        {
            return TOP_LEFT;
        }
		else if (x >= (max_x / 2) && y < (max_y / 2))
		{
			return TOP_RIGHT;
		}
        else if (x < (max_x / 2) && y >= (max_y / 2))
        {
            return BOTTOM_LEFT;
        }
        else if (x >= (max_x / 2) && y >= (max_y / 2))
        {
            return BOTTOM_RIGHT;
        }
        return NONE;
    }
    int64_t x = 0;
    int64_t y = 0;
};

class Robot
{
public:
	Robot() = default;
    Robot(int64_t x, int64_t y, int64_t vx, int64_t vy)
		: m_location{x, y}, m_velocity{vx, vy} 
	{
	
	}
	Robot(const Point& location, const Point& velocity)
		: m_location(location), m_velocity(velocity) 
    {
    
    }

	void Move()
	{
		m_location.x += m_velocity.x;
		m_location.y += m_velocity.y;

        // wrap around
        if (m_location.x < 0)
        { 
            m_location.x += max_x;
        }
        else if (m_location.x >= max_x)
		{
			m_location.x -= max_x;
		}

        if (m_location.y < 0)
		{
			m_location.y += max_y;
		}
		else if (m_location.y >= max_y)
		{
			m_location.y -= max_y;
		}
	}

	const Point& GetLocation() const { return m_location; }
	const Point& GetVelocity() const { return m_velocity; }
private:
    Point m_location;
    Point m_velocity;
};

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

    // Regex to match lines like: "p=0,4 v=3,-3" (including negatives)
    std::regex line_rx(R"(p=(-?\d+),(-?\d+)\s+v=(-?\d+),(-?\d+))");
    std::smatch match;
    std::string line;

    std::vector<Robot> robots;
    while (std::getline(input_file, line))
    {
        if (line.empty())
        {
            continue;
        }

        if (std::regex_match(line, match, line_rx))
        {
            int64_t p_x = std::stoi(match[1].str());
            int64_t p_y = std::stoi(match[2].str());
            int64_t v_x = std::stoi(match[3].str());
            int64_t v_y = std::stoi(match[4].str());
            robots.emplace_back(Robot{p_x, p_y, v_x, v_y});
        }
        else
        {
            std::cerr << "Line did not match expected format: " << line << "\n";
            return 1;
        }
    }

    for (int i = 0; i < 100; ++i)
    {
        for (Robot& robot : robots)
        {
            robot.Move();
        }
    }

    std::map<Quadrant, int64_t> quadrant_counts;
    for (const Robot& robot : robots)
	{
		Quadrant q = robot.GetLocation().GetQuadrant();
		if (q != NONE)
		{
			quadrant_counts[q]++;
		}
	}

    int64_t safety_factor = 1;
    for (const auto& [quadrant, count] : quadrant_counts)
	{
        safety_factor *= count;
        std::cout << "Quadrant " << quadrant << ": " << count << " robots\n";
	}

    std::cout << "Safety Factor: " << safety_factor << std::endl;

    return 0;
}
