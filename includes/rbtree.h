#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <fstream>

const int NUM_SUGGESTIONS = 5;

struct rbNode{
    std::string word;
    rbNode* left;
    rbNode* right;
    rbNode* parent;
    bool color;
};

class RBTree {
public:

    RBTree();
    ~RBTree();

    void make(const std::string& filename);
    void find_prefix(const std::string& prefix);


private:


    void insert_node(const std::string& word);
    void insert_fixup(rbNode* node);
    void rotate_left(rbNode* node);
    void rotate_right(rbNode* node);
    void destroy_tree(rbNode* node);

    rbNode* root_;

};

#endif