#pragma once

#include <regex>
#include <string>
#include <memory>

struct Point {
    int x;
    int y;

    const static std::regex inputFileRegex;

    Point() : x(0), y(0) {};
    Point(const int& x, const int& y) : x(x), y(y) {};
    Point(const std::string& inputLine);

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    };

    bool operator!=(const Point& other) const
    {
        return x != other.x || y != other.y;
    }

    int operator-(const Point& other) const
    {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }

    Point operator+(const Point& other) const
    {
        return Point(x + other.x, y + other.y);
    }

    Point operator/(const int& div) const
    {
        return Point(x / div, y / div);
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& pt);

    void getAdjacent(std::vector<Point>& adj) const;
};

namespace std {
    template<> struct hash<Point>
    {
        std::size_t	operator()(const Point& p) const noexcept
        {
            return std::hash<int>()(p.x + 10 * p.y);
        }
    };
}