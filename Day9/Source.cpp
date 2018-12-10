#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

inline void advanceLoop(
    std::list<uint32_t>::iterator& it, 
    const std::list<uint32_t>::iterator& begin, 
    const std::list<uint32_t>::iterator& end, 
    int nPos
)
{
    while (nPos > 0)
    {
        if (it == end) { it = begin; }
        it++;
        nPos--;
    }
    while (nPos < 0)
    {
        if (it == begin) { it = end; }
        it--;
        nPos++;
    }
}

uint32_t getHighscore(int nPlayers, uint32_t lastScore)
{
    std::vector<uint32_t> scores(nPlayers, 0);
    std::list<uint32_t> marbles = { 0 };
    std::list<uint32_t>::iterator listIterator = marbles.begin();
    long marbleValue = 1;
    int currentPlayer = 0;
    while (marbleValue - 1 != lastScore)
    {
        if (marbleValue % 23 != 0)
        {
            advanceLoop(listIterator, marbles.begin(), marbles.end(), 2);
            listIterator = marbles.insert(listIterator, marbleValue++);
        }
        else
        {
            advanceLoop(listIterator, marbles.begin(), marbles.end(), -7);
            scores[currentPlayer] += *listIterator + marbleValue++;
            listIterator = marbles.erase(listIterator);
        }
        currentPlayer = (currentPlayer + 1) % nPlayers;
    }

    return *std::max_element(scores.begin(), scores.end());
}

int main(int argc, char** argv)
{
    const std::regex inputRegex(R"((\d+) players; last marble is worth (\d+) points)");
    std::ifstream inputFile;
    inputFile.open("input.txt");
    if (inputFile.is_open())
    {
        std::string line;
        std::getline(inputFile, line);

        std::smatch match;
        if (std::regex_search(line, match, inputRegex))
        {
            int nPlayers = std::stoi(match[1]), lastScore = std::stoi(match[2]);
            
            std::cout << "Highest score is " << getHighscore(nPlayers, lastScore) << std::endl;
            std::cout << "'You just want me to get smart don't you' score is " << getHighscore(nPlayers, lastScore * 100) << std::endl;
        }
    }

    std::cin.get();
}