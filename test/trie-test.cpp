#include "../includes/trie.h"


int main() {

  Trie trie;

  trie.readFromFile("assets/data/test.txt");
  trie.printTrie();

  return  0;
}