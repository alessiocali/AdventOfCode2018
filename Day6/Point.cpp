#include "Point.h"

const std::regex Point::inputFileRegex(R"((\d+), (\d+))");

Point::Point(const std::string& inputLine)
{
    std::smatch match;
    if (std::regex_search(inputLine, match, inputFileRegex))
    {
        x = std::stoi(match[1]);
        y = std::stoi(match[2]);
    }
    else
    {
        throw "Invalid Point regex";
    }
}

void Point::getAdjacent(std::vector<Point>& adj) const
{
    adj.clear();
    adj.push_back(Point(x - 1, y));
    adj.push_back(Point(x + 1, y));
    adj.push_back(Point(x, y - 1));
    adj.push_back(Point(x, y + 1));
}

std::ostream& operator<<(std::ostream& os, const Point& pt)
{
    os << "(" << pt.x << ", " << pt.y << ")";
    return os;
}