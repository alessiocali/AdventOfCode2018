#pragma once

#include <map>
#include <memory>
#include <vector>

class Node
{
public:
    Node(char c) : value(c) { };

    void append(std::shared_ptr<Node> node);
    void prepend(std::shared_ptr<Node> node);
    bool isRoot() const;
    char getValue() const;
    void removeRequirement(char key);
    bool hasRequirements();
    void complete(std::vector<std::shared_ptr<Node>>& readyNodes);

private:

    char value;
    std::map<char, std::shared_ptr<Node>> next;
    std::map<char, std::shared_ptr<Node>> prev;
};
