#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define OUT

void isTwiceOrThrice(const std::string& line, OUT bool& twice, OUT bool& thrice)
{
    twice = thrice = false;
    std::string sorted = line;
    std::sort(sorted.begin(), sorted.end());

    int count = 0;
    char current = sorted[0];
    for (std::string::const_iterator it = sorted.begin() + 1; !(twice && thrice) && it != sorted.end(); it++)
    {
        if (*it != current)
        {
            if (count == 2) { twice = true; }
            if (count == 3) { thrice = true; }
            current = *it;
            count = 0;
        }
        count++;
    }
}

// Check if two strings differ by only one character, match will be the same characters
// string. Will be empty if characters differ by more or less than one character
void doMatch(const std::string& s1, const std::string& s2, OUT std::string& match)
{
    match.clear();
    if (s1.length() != s2.length()) { return; }

    int diffCount = 0;
    std::string::const_iterator it1 = s1.begin(), it2 = s2.begin();
    std::string::const_iterator end1 = s1.end(), end2 = s2.end();
    for
    (
        std::string::const_iterator it1 = s1.begin(), it2 = s2.begin() ;
        diffCount < 2 && it1 != end1 && it2 != end2 ;
        it1++, it2++
    ) {
        if (*it1 != *it2) { diffCount++; }
        else { match += *it1; }
    }

    if (diffCount != 1) { match.clear(); }
}

int main(int argc, char** argv)
{
    std::ifstream inputFile;
    inputFile.open("input.txt");
    std::string line;

    std::vector<std::string> tags;
    int twiceCount, thriceCount;
    twiceCount = thriceCount = 0;
    if (inputFile.is_open())
    {
        while (std::getline(inputFile, line))
        {
            bool isTwice, isThrice;
            isTwiceOrThrice(line, isTwice, isThrice);
            if (isTwice) { twiceCount++; }
            if (isThrice) { thriceCount++; }
            tags.push_back(std::move(line));
        }
    }

    std::cout << "Checksum: " << (twiceCount * thriceCount) << std::endl;

    std::vector<std::string>::const_iterator s1It, s2It;
    std::string::const_iterator diff;
    std::string match = "";
    for (std::vector<std::string>::const_iterator it = tags.begin(); it != tags.end() && match.length() == 0; it++)
    {
        for (std::vector<std::string>::const_iterator jt = it + 1; jt != tags.end() && match.length() == 0; jt++)
        {
            doMatch(*it, *jt, match);
            if (match.length() > 0)
            {
                s1It = it;
                s2It = jt;
            }
        }
    }

    if (match.length() > 0)
    {
        std::cout << "Matching strings are: " << std::endl << *s1It << std::endl << *s2It << std::endl;
        std::cout << "Matching characters are: " << match << std::endl;
    }

    system("pause");
}