#include <iostream>
#include <string>
#include <fstream>
#include <set>

int main(int argc, char** argv)
{
    std::ifstream inputFile;
    inputFile.open("input.txt");

    bool repeatedFrequencyFound = false;
    int sum = 0, repeatedFrequency;
    std::set<int> reachedFrequencies;
    if (inputFile.is_open())
    {
        int val;
        while (inputFile >> val)
        {
            sum += val;
            if (!repeatedFrequencyFound)
            {
                if (reachedFrequencies.count(sum) == 0)
                {
                    reachedFrequencies.insert(sum);
                }
                else
                {
                    repeatedFrequency = sum;
                    repeatedFrequencyFound = true;
                }
            }
        }
    }

    std::cout << "Resulting frequency is " << sum << std::endl;
    if (repeatedFrequencyFound)
    {
        std::cout << "First repeating frequency is " << repeatedFrequency << std::endl;
    }
    system("pause");
}