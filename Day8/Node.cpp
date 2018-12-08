#include "Node.h"

#include <iostream>
#include <numeric>

int Node::getCompleteMetadataSum() const
{
    return std::accumulate(children.begin(), children.end(), getOwnMetadataSum(),
        [](const int& op1, const Node& op2) { return op1 + op2.getCompleteMetadataSum(); }
    );
}

int Node::value() const
{
    if (childCount == 0)
    {
        return getOwnMetadataSum();
    }
    else
    {
        int value = 0;
        for (const int& meta : metadata)
        {
            if (meta > 0 && static_cast<unsigned int>(meta) <= childCount)
            {
                value += children[meta - 1].value();
            }
        }
        return value;
    }
}

inline int Node::getOwnMetadataSum() const
{
    return std::accumulate(metadata.begin(), metadata.end(), 0);
}

std::istream& operator>>(std::istream& is, Node& node)
{
    is >> node.childCount >> node.metadataCount;
    node.children.reserve(node.childCount);

    for (std::size_t i = 0; i < node.childCount; i++)
    {
        Node child;
        is >> child;
        node.children.push_back(std::move(child));
    }

    node.metadata.reserve(node.metadataCount);
    for (std::size_t i = 0; i < node.metadataCount; i++)
    {
        int meta;
        is >> meta;
        node.metadata.push_back(std::move(meta));
    }

    return is;
}