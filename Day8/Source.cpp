#include <fstream>
#include <iostream>

#include "Node.h"

int main(int argc, char** argv)
{
    std::ifstream inputFile;
    inputFile.open("input.txt");
    Node root;
    if (inputFile.is_open())
    {
        inputFile >> root;
    }

    std::cout << "Metadata count is " << root.getCompleteMetadataSum() << std::endl;
    std::cout << "Value of root is " << root.value() << std::endl;
    system("pause");
}