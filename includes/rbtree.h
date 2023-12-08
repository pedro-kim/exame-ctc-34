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

class RBTree
{
public:
  RBTree();
  // ~RBTree();

  void make(const std::string &filename);
  std::vector<std::string> find_prefix(const std::string &prefix);
  void estimateMemoryUsage();
  std::vector<std::string> search_prefix(const std::string &prefix);

private:

  int numberOfNodes_;
  class Node
  {
  public:
    Node(const std::string &word)
    {
      rightchild = nullptr;
      leftchild = nullptr;
      parent = nullptr;
      color = RED;
      _word = word;
    }

    Node *parent;
    Node *leftchild;
    Node *rightchild;

    enum Color
    {
      RED,
      BLACK
    };
    Color color;

    std::string word() { return _word; }

  private:
    std::string _word;
  };

  void add(const std::string &word);
  void rotateRight(Node *node);
  void rotateLeft(Node *node);
  void find_prefix_recursive(Node *node, const std::string &prefix, std::vector<std::string> &suggestions);
  void search_recursive (Node* current, const std::string &first, const std::string &last, std::vector<std::string> suggestions, const std::string &prefix);
  // void destroy_tree(rbNode* node);

  Node *_root;
};

#endif