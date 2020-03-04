#ifndef TREE_H
#define TREE_H

#include <string>
#include <stddef.h>

#include "node.h"

// https://en.wikipedia.org/wiki/B-tree

// Every node has at most m children.
// Every non-leaf node (except root) has at least ⌈m/2⌉ child nodes.
// The root has at least two children if it is not a leaf node.
// A non-leaf node with k children contains k − 1 keys.
// All leaves appear in the same level and carry no information.

class Tree {
public:
    // const size_t max_children = 2; // The amount of nodes which can be in 1 node
    Tree();
    ~Tree();
    bool insert(std::string);
    bool contains(std::string);
private:
    Node* root;
    bool insert(std::string url, Node* pointer);
    void destroy(Node* node);
};
#endif