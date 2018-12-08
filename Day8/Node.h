#pragma once

#include <vector>

class Node
{
public:
    Node() : childCount(0), metadataCount(0) { };
    
    int getCompleteMetadataSum() const;
    int value() const;
    friend std::istream& operator>>(std::istream& is, Node& node);

private:
    inline int getOwnMetadataSum() const;

    unsigned int childCount, metadataCount;
    std::vector<int> metadata;
    // Nesting rather than referencing is intentional
    std::vector<Node> children;
};

std::istream& operator>>(std::istream& is, Node& node);