#ifndef TREE_H
#define TREE_H

#include <string>
#include <stddef.h>

#include "node.h"


class Tree {
public:
    Tree();
    ~Tree();
    bool insert(std::string);
private:
    Node* root;
    bool insert(std::string url, Node* pointer);
    void destroy(Node* node);
};
#endif