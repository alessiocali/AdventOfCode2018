#include "Point.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Compute likeliness that points form a sensible message by computing 
// sum of vertically and horizontally contiguous points
int getScore(std::vector<Point>& points)
{
    int score = 0;

    // Horizontal
    std::sort(points.begin(), points.end(), byRow);
    std::vector<Point>::const_iterator prev = points.begin();
    for (std::vector<Point>::const_iterator it = ++(points.begin()); it != points.end(); it++)
    {
        if (it->y == prev->y && it->x == prev->x + 1)
        {
            score++;
        }
        prev = it;
    }

    // Vertical
    std::sort(points.begin(), points.end(), byCol);
    prev = points.begin();
    for (std::vector<Point>::const_iterator it = ++(points.begin()); it != points.end(); it++)
    {
        if (it->x == prev->x && it->y == prev->y + 1)
        {
            score++;
        }
        prev = it;
    }

    return score;
}

void printvector(std::vector<Point>& points)
{
    int minX, minY, maxX, maxY;
    minX = minY = std::numeric_limits<int>::max();
    maxX = maxY = std::numeric_limits<int>::min();

    for (const Point& point : points)
    {
        if (point.x < minX) { minX = point.x; }
        if (point.x > maxX) { maxX = point.x; }
        if (point.y < minY) { minY = point.y; }
        if (point.y > maxY) { maxY = point.y; }
    }

    // Init message vector
    std::vector<std::string> message(maxY - minY + 1, std::string(maxX - minX + 1, '.'));
    for (const Point& point : points)
    {
        message[point.y - minY][point.x - minX] = '#';
    }

    for (const std::string& line : message)
    {
        std::cout << line << std::endl;
    }
}

void moveAll(std::vector<Point>& points)
{
    std::for_each(points.begin(), points.end(), [](Point& pt) { pt.move(); });
}

int main(int argc, char** argv)
{
    std::vector<Point> points;
    
    std::ifstream inputFile;
    inputFile.open("input.txt");
    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            points.push_back(Point(line));
        }
    }

    const int MAX_ITERATIONS = 50000;
    const int THRESHOLD = 50;

    // Step time and compute score. Print messages above a given threshold.
    for (int i = 0; i < MAX_ITERATIONS; i++)
    {
        int vectorScore = getScore(points);
        if (vectorScore > THRESHOLD)
        {
            std::cout << std::endl;
            printvector(points);
            std::cout << "Message score " << vectorScore << " at iteration " << i << std::endl;
            std::cin.get();
        }
        moveAll(points);
        std::cout << "Iteration " << i << "\r";
    }

    std::cout << "Computed all iterations" << std::endl;
    std::cin.get();
}