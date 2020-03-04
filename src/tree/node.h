#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

struct Node;

struct Node {
    Node* left;
    Node* right;
    std::string val;
    Node(std::string url);
};
#endif