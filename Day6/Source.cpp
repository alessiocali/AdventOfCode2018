#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <functional>
#include <algorithm>
#include <numeric>

#include "Point.h"

#define OUT

const int MAX_SAFE_DST = 10000;

bool getArea(const Point& base, const std::vector<Point>& all, OUT std::unordered_set<Point>& area)
{
    area.clear();

    std::vector<Point> toExplore;
    // Push a copy which will be kept in the point area
    toExplore.push_back(base);

    std::vector<Point> adjacents;
    while (toExplore.size() > 0)
    {
        // To avoid copying the current value (and using pointers),
        // just move toExplore's tail.
        Point current = std::move(toExplore.back());
        toExplore.pop_back();
        
        // If this Point has already been explored, move on
        if (area.count(current)) { continue; }

        // Check if this Point belongs to the base's area.
        int dstFromBase = current - base;
        bool closestToOther = std::any_of( all.begin(), all.end(),
            [current, base, dstFromBase](const Point& otherPt) 
            { 
                return base != otherPt && current - otherPt <= dstFromBase;
            }
        );
        if (closestToOther) { continue; }

        // The point is in our area but... Is it any closer to any of
        // the other targets, compared to the base?
        bool dstFromAnyOtherDecreased = std::any_of(all.begin(), all.end(),
            [current, base](const Point& otherPt)
            {
                return base != otherPt && current - otherPt <= base - otherPt;
            }
        );

        // Nope, we're straying further from every other target.
        // -> This area is unbounded
        if (!dstFromAnyOtherDecreased) { return false; }
        else 
        { 
            // Valid point! Expand to its adjacents and move it 
            // to the base's area
            current.getAdjacent(adjacents);
            for (const Point& adj : adjacents) { toExplore.push_back(std::move(adj)); }
            area.insert(std::move(current));
        }
    }

    return true;
}

void getSafeArea(const std::vector<Point>& points, OUT std::unordered_set<Point>& area)
{
    int minX, minY, maxX, maxY;
    minX = minY = std::numeric_limits<int>::max();
    maxX = maxY = std::numeric_limits<int>::min();

    std::vector<Point> toExplore;
    std::vector<Point> adjacents;
    toExplore.push_back(std::accumulate(points.begin(), points.end(), Point()) / points.size());

    while (toExplore.size() > 0)
    {
        Point current = std::move(toExplore.back());
        toExplore.pop_back();

        if (area.count(current)) { continue; }

        int cumDistance = 0;
        for (std::vector<Point>::const_iterator it = points.begin(); cumDistance < MAX_SAFE_DST && it != points.end(); it++)
        {
            cumDistance += *it - current;
        }
        if (cumDistance < MAX_SAFE_DST)
        {
            current.getAdjacent(adjacents);
            for (const Point& adj : adjacents) { toExplore.push_back(std::move(adj)); }
            area.insert(std::move(current));
        }
    }
}

int main(int argc, char** argv)
{
    std::unordered_map<Point, std::unordered_set<Point>> output;
    std::vector<Point> points;

    // Init list of points
    std::ifstream inFile;
    std::string line;
    inFile.open("input.txt");
    if (!inFile.is_open())
    {
        std::cerr << "File IO error" << std::endl;
        return 1;
    }
    while (std::getline(inFile, line))
    {
        points.push_back(Point(line));
    }

    // Detect areas for each point
    for (const Point& pt : points)
    {
        std::unordered_set<Point> out;

        std::cout << "Area of " << pt;
        if (!getArea(pt, points, out))
        {
            std::cout << " is unbounded";
        }
        else
        {
            std::cout << " is " << out.size();
            output[pt] = std::move(out);
        }
        std::cout << std::endl;
    }

    // Find biggest area
    auto maxArea = std::max_element(output.begin(), output.end(),
        [](const auto& pair1, const auto& pair2) 
        { 
            return pair1.second.size() < pair2.second.size(); 
        }
    );

    std::cout << "Max element at point " << maxArea->first << " (value " << maxArea->second.size() << ")" << std::endl;

    std::unordered_set<Point> safeArea;
    getSafeArea(points, safeArea);

    std::cout << "Safe area has size " << safeArea.size() << std::endl;

    system("pause");
    return 0;
}