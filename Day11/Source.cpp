#include <iostream>
#include <array>
#include <numeric>

// Brute force solution, not optimal at all, but works

int getPowerLevel(int x, int y, int gridSN)
{
    int rackId = x + 10;
    int powerLevel = (rackId * y + gridSN) * rackId;
    return ( (powerLevel / 100) % 10 ) - 5;
}

void fillGrid(std::array<std::array<int, 300>, 300>& grid, int gridSN)
{
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < 300; j++)
        {
            grid[i][j] = getPowerLevel(i + 1, j + 1, gridSN);
        }
    }
}

int getSurroundingScore(const std::array<std::array<int, 300>, 300>& grid, int x, int y, int s = 3)
{
    x--; y--;   // Reduce coordinates to indices
    if (x + s > 300 || y + s > 300)
    {
        return std::numeric_limits<int>::min();
    }

    return std::accumulate(grid.begin() + x, grid.begin() + x + s, 0, 
        [y, s](int& sum, const std::array<int, 300>& col)
        {
            return sum + std::accumulate(col.begin() + y, col.begin() + y + s, 0);
        }
    );
}

int getBestScoreAtCell(const std::array<std::array<int, 300>, 300>& grid, int x, int y, int& bS)
{
    int bestScore = std::numeric_limits<int>::min();
    for (int s = 1; s <= 300; s++)
    {
        int score = getSurroundingScore(grid, x, y, s);
        if (score > bestScore)
        {
            bestScore = score;
            bS = s;
        }
    }

    return bestScore;
}

int main(int argc, char** argv)
{
    const int gridSN = 9435;
    std::array<std::array<int, 300>, 300> grid;
    fillGrid(grid, gridSN);

    int bX3 = -1, bY3 = -1, bX = -1, bY = -1, bS;
    int bestScore3 = std::numeric_limits<int>::min();
    int bestScoreAll = std::numeric_limits<int>::min();
    int s;
    for (int x = 1; x <= 300; x++)
    {
        for (int y = 1; y <= 300; y++)
        {
            int score = getSurroundingScore(grid, x, y);
            if (score > bestScore3)
            {
                bestScore3 = score;
                bX3 = x;
                bY3 = y;
            }
            
            score = getBestScoreAtCell(grid, x, y, s);
            if (score > bestScoreAll)
            {
                bestScoreAll = score;
                bX = x;
                bY = y;
                bS = s;
            }
        }
    }

    std::cout << "Best score in 3x3 grid is " << bestScore3 << " at (" << bX3 << ", " << bY3 << ")" << std::endl;
    std::cout << "Best score in any grid is " << bestScoreAll << " at (" << bX << ", " << bY << "), size " << bS << std::endl;
    std::cin.get();
}