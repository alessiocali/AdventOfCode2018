#include "Node.h"

void Node::append(std::shared_ptr<Node> node)
{
    if (!next.count(node->value))
    {
        next[node->value] = node;
    }
}

void Node::prepend(std::shared_ptr<Node> node)
{
    if (!prev.count(node->value))
    {
        prev[node->value] = node;
    }
}

bool Node::isRoot() const
{
    return prev.size() == 0;
}

char Node::getValue() const
{
    return value;
}

void Node::removeRequirement(char key)
{
    prev.erase(key);
}

bool Node::hasRequirements()
{
    return prev.size() > 0;
}

void Node::complete(std::vector<std::shared_ptr<Node>>& readyNodes)
{
    readyNodes.clear();
    for (auto& pair : next)
    {
        pair.second->removeRequirement(value);
        if (!pair.second->hasRequirements())
        {
            readyNodes.push_back(pair.second);
        }
    }
    next.clear();
}