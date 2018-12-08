#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>

#define OUT

struct Rectangle 
{
    Rectangle() : x(0), y(0), w(0), h(0) {};
    Rectangle(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    int x, y, w, h;
};

void loadFile(const std::string& filename, OUT std::unordered_map<std::string, Rectangle>& claims, OUT int& fabricW, OUT int& fabricH)
{
    const std::regex inputRegex("#(\\w+) @ (\\d+),(\\d+): (\\d+)x(\\d+)");
    std::smatch inputMatches;

    std::string line;
    std::ifstream inputFile;
    inputFile.open("input.in");

    if (inputFile.is_open())
    {
        claims.clear();
        std::string line;
        fabricW = 1000, fabricH = 1000;

        while (std::getline(inputFile, line))
        {
            if (std::regex_search(line, inputMatches, inputRegex))
            {
                int x = stoi(inputMatches[2]);
                int y = stoi(inputMatches[3]);
                int w = stoi(inputMatches[4]);
                int h = stoi(inputMatches[5]);

                claims[inputMatches[1]] = Rectangle(x, y, w, h);

                if (x + w > fabricW) { fabricW = x + w; }
                if (y + h > fabricH) { fabricH = y + h; }
            }
        }
    }
}

void fillFabric(std::vector<std::vector<int>>& fabric, const std::unordered_map<std::string, Rectangle>& claims)
{
    for (
        std::unordered_map<std::string, Rectangle>::const_iterator it = claims.begin();
        it != claims.end();
        it++
    ) {
        const Rectangle& rect = it->second;
        for (int i = rect.x; i < rect.x + rect.w; i++)
        {
            for (int j = rect.y; j < rect.y + rect.h; j++)
            {
                fabric[i][j]++;
            }
        }
    }
 }

std::string getIsolatedClaim(const std::vector<std::vector<int>>& fabric, const std::unordered_map<std::string, Rectangle>& claims)
{
    for (
        std::unordered_map<std::string, Rectangle>::const_iterator it = claims.begin();
        it != claims.end();
        it++
    ) {
        const Rectangle& rect = it->second;
        bool claimed = std::any_of(fabric.begin() + rect.x , fabric.begin() + rect.x + rect.w,
            [rect](const std::vector<int>& col)
            {
                return std::any_of(col.begin() + rect.y, col.begin() + rect.y + rect.h,
                    [](const int& claims) { return claims > 1; }
                );
            }
        );

        if (!claimed) 
        {
            return it->first;
        }
    }

    return "NotFound";
}

int getOverclaimedCount(const std::vector<std::vector<int>>& fabric)
{
    int count = 0;
    for (const std::vector<int>& col : fabric)
    {
        count += std::count_if(col.begin(), col.end(), 
            [](const int& claim) { return claim > 1; }
        );
    }
    return count;
}

int main(int argc, char** argv)
{
    int fabricW, fabricH;
    std::unordered_map<std::string, Rectangle> claims;

    loadFile("input.in", claims, fabricW, fabricH);

    std::vector<std::vector<int>> fabric(fabricW, std::vector<int>(fabricH, 0));
    fillFabric(fabric, claims);

    std::cout << getOverclaimedCount(fabric) << std::endl;
    std::cout << getIsolatedClaim(fabric, claims) << std::endl;

    std::cin.get();
}