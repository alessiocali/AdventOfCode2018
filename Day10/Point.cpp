#include "Point.h"

#include <string>

const std::regex Point::inputRegex = std::regex(R"(position=<\s*(\-?\d+),\s*(\-?\d+)> velocity=<\s*(\-?\d+),\s*(\-?\d+)>)");

void Point::move()
{
    x += vx;
    y += vy;
}

Point::Point(const std::string& inputLine) : Point::Point()
{
    std::smatch match;

    if (std::regex_search(inputLine, match, Point::inputRegex))
    {
        x = std::stoi(match[1]);
        y = std::stoi(match[2]);
        vx = std::stoi(match[3]);
        vy = std::stoi(match[4]);
    }
}

bool byRow(const Point& p1, const Point& p2)
{
    return p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x);
}

bool byCol(const Point& p1, const Point& p2)
{
    return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
}