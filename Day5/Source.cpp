#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#define OUT

bool inline canReact(const char& c1, const char& c2)
{
    return c1 != c2 && std::toupper(c1) == std::toupper(c2);
}

void reduce(std::istream& input, OUT std::string& output)
{
    char current;
    bool eof = false;
    output.clear();

    while (!eof && input.get(current))
    {
        if (output.empty() || !canReact(output.back(), current))
        {
            output += current;
        }
        else
        {
            output.pop_back();
        }
    }
}

void getReducedOptions(const std::string& polymer, OUT std::vector<std::string>& options)
{
    options.clear();
    for (char c = 'a'; c <= 'z'; c++)
    {
        std::string newPoly = polymer;
        newPoly.erase(
            std::remove_if(
                newPoly.begin(), 
                newPoly.end(), 
                [c](const char& cc) { return std::toupper(c) == std::toupper(cc); }
            ),
            newPoly.end()
        );

        std::stringstream newPolyStream(newPoly);
        std::string reducedPoly;
        reduce(newPolyStream, reducedPoly);
        options.push_back(reducedPoly);
    }
}

int main(int argc, char** argv)
{
    std::ifstream inputFile;
    std::string reducedPolymer;
    bool eof = false;
    
    inputFile.open("input.in");
    if (inputFile.is_open())
    {
        reduce(inputFile, reducedPolymer);
        std::cout << reducedPolymer.length() << std::endl;

        std::vector<std::string> options;
        getReducedOptions(reducedPolymer, options);
        std::vector<std::string>::const_iterator shortest = 
            std::min_element(
                options.begin(), 
                options.end(),
                [](const std::string& s1, const std::string s2) { return s1.length() < s2.length(); }
            );

        std::cout << shortest->length() << std::endl;

        std::cin.get();
    }
}