#include "tree.h"
#include "node.h"

/**
//          uni.nl
//         /      \
//      aa.nl     zz.nl

// Add dd.nl:
//          uni.nl
//         /      \
//      dd.nl     zz.nl
//     /
//  aa.nl

// Add cc.nl:
//          uni.nl
//         /      \
//      cc.nl     zz.nl
//     /    \
//  aa.nl    dd.nl


// Situations: depth = 1
//       uni.nl

// Situations: depth = 2
//       uni.nl
//      /
//    aa.nl

// Situations: depth = 2
//       dd.nl
//      /      \
//    aa.nl    uni.nl
**/


Tree::Tree() {
    root = NULL;
}

bool Tree::insert(std::string url) {
    if (root == NULL) {
        root = new Node(url);
        return true;
    }
    return insert(url, root);
}

bool Tree::insert(std::string url, Node* pointer) {
    int cmp = url.compare(pointer->val);

    if (cmp < 0) {
        if (pointer->left != NULL) {
            return insert(url, pointer->left);
        } else {
            pointer->left = new Node(url);
            return true;
        }
    } else if (cmp > 0) {
        if (pointer->right != NULL) {
            return insert(url, pointer->right);
        } else {
            pointer->right = new Node(url);
            return true;
        }
    } else {
        return false;
    }
}

void Tree::destroy(Node* node) {
    if (node == NULL)
        return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

Tree::~Tree() {
    destroy(root);
}