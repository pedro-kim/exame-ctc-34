#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <fstream>

const int NUM_SUGGESTIONS = 100;

class RBTree {
public:

    RBTree();
    // ~RBTree();

    void make(const std::string& filename);
    void find_prefix(const std::string& prefix);


private:

    class Node {
            public:
 
        Node(const std::string& word) {
            rightchild = nullptr;
            leftchild = nullptr;
            parent = nullptr;
            color = RED;
            _word = word;
        }

        Node* parent;
        Node* leftchild;
        Node* rightchild;

        enum Color { RED, BLACK };
        Color color;

        std::string word () {return _word;}


        private:

        std::string _word;

    };

    void add(const std::string& word);
    void rotateRight(Node* node);
    void rotateLeft(Node* node);
    void find_prefix_recursive(Node* node, const std::string& prefix, std::vector<std::string>& suggestions);
    //void destroy_tree(rbNode* node);

    Node* _root;

};

#endif