#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <unordered_map>
#include <string>

#include "Node.h"

void loadFile(const std::string& filename, std::unordered_map<char, std::shared_ptr<Node>>& nodeMap)
{
    const static std::regex inputRegex("Step ([A-Z]) must be finished before step ([A-Z]) can begin.");
    std::smatch match;

    std::ifstream inputFile;
    inputFile.open(filename);

    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            if (std::regex_search(line, match, inputRegex))
            {
                char before = match[1].str()[0];
                char after = match[2].str()[0];

                if (nodeMap.count(before) == 0)
                {
                    nodeMap[before] = std::make_shared<Node>(before);
                }
                if (nodeMap.count(after) == 0)
                {
                    nodeMap[after] = std::make_unique<Node>(after);
                }

                nodeMap[before]->append(nodeMap[after]);
                nodeMap[after]->prepend(nodeMap[before]);
            }
        }
    }
}

void getRoots(const std::unordered_map<char, std::shared_ptr<Node>>& nodeMap, std::map<char, std::shared_ptr<Node>>& roots)
{
    for (const auto& pair : nodeMap)
    {
        if (pair.second->isRoot())
        {
            roots[pair.second->getValue()] = pair.second;
        }
    }
}

void getExecutionSequence(const std::unordered_map<char, std::shared_ptr<Node>>& nodeMap, std::string& sequence)
{
    sequence.clear();

    std::map<char, std::shared_ptr<Node>> readyNodes;
    getRoots(nodeMap, readyNodes);

    std::vector<std::shared_ptr<Node>> newReadyNodes;
    while (readyNodes.size() > 0)
    {
        auto it = readyNodes.begin();
        sequence += it->first;
        it->second->complete(newReadyNodes);
        for (const auto& newReadyPtr : newReadyNodes)
        {
            readyNodes[newReadyPtr->getValue()] = newReadyPtr;
        }
        readyNodes.erase(it->first);
    }
}

int getParallelExecutionSequence
(
    const std::unordered_map<char, std::shared_ptr<Node>>& nodeMap, 
    std::string& sequence, 
    const size_t maxWorkers = 5, 
    const int baseDelay = 60
) {
    sequence.clear();

    // Map instants in times to nodes which get completed at that time
    std::multimap<int, std::shared_ptr<Node>> executionMap;

    // Nodes pending execution
    std::map<char, std::shared_ptr<Node>> readyNodes;
    getRoots(nodeMap, readyNodes);

    std::vector<std::shared_ptr<Node>> newReadyNodes;
    int totalExecTime = 0;
    while (readyNodes.size() > 0 || executionMap.size() > 0)
    {
        // Move ready nodes to execution
        std::map<char, std::shared_ptr<Node>>::const_iterator it;
        for (it = readyNodes.begin(); executionMap.size() < maxWorkers && it != readyNodes.end(); it++)
        {
            int execTime = totalExecTime + baseDelay + (it->first - 'A' + 1);
            executionMap.insert(std::make_pair(execTime, it->second));
        }
        readyNodes.erase(readyNodes.begin(), it);

        // Move forward to next execution step
        if (executionMap.size() > 0)
        {
            totalExecTime = executionMap.begin()->first;

            // Process completed nodes at this time        
            auto range = executionMap.equal_range(totalExecTime);
            auto& it = range.first;
            auto& end = range.second;
            for ( ; it != end; it++)
            {
                std::shared_ptr<Node>& nodePtr = it->second;
                sequence += nodePtr->getValue();

                nodePtr->complete(newReadyNodes);
                for (const auto& newReadyPtr : newReadyNodes)
                {
                    readyNodes[newReadyPtr->getValue()] = newReadyPtr;
                }
            }
            executionMap.erase(totalExecTime);
        }  
    }

    return totalExecTime;
}

int main(int argc, char** argv)
{
    std::unordered_map<char, std::shared_ptr<Node>> nodeMap;
    loadFile("input.txt", nodeMap);

    std::string sequence = "";
    getExecutionSequence(nodeMap, sequence);
    std::cout << "Sequence: " << sequence << std::endl;

    // Reload map. Too lazy to change algorithm so that it doesn't alter the graph
    loadFile("input.txt", nodeMap);
    int execTime = getParallelExecutionSequence(nodeMap, sequence);
    std::cout << "Sequence: " << sequence << " completed in " << execTime << " seconds." << std::endl;

    std::cin.get();
    return 0;
}