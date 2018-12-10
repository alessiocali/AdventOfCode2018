#pragma once

#include <regex>

struct Point
{
    Point() : x(0), y(0), vx(0), vy(0) { };
    Point(const std::string &inputLine);

    void move();

    const static std::regex inputRegex;
    int x, y, vx, vy;
};

bool byCol(const Point& p1, const Point& p2);
bool byRow(const Point& p1, const Point& p2);