#include <iostream>
#include "../includes/transducer.h"
#include "../includes/trie.h"
#include "../includes/levenshtein.h"

int transducer_test() {
  std::cout << "Hello World! FST" << std::endl;
  return 0;
}

int trie_test() {

  std::cout << "Hello World! Trie" << std::endl;

  return  0;
}

#include <iostream>

int levenshtein_test() {
  std::cout << "Hello World! Levenshtein" << std::endl;
  return 0;
}


int main() {
  std::cout << "Hello World! Main" << std::endl;

  bool valid_test = false;

  while (!valid_test) {
    std::string test;
    std::cout << "Choose a test (fst, trie, lev): ";
    std::cin >> test;

    if (test == "fst") {
      transducer_test();
      valid_test = true;
    } else if (test == "trie") {
      trie_test();
      valid_test = true;
    } else if (test == "lev") {
      levenshtein_test();
      valid_test = true;
    } else {
      std::cout << "Invalid test. Try again." << std::endl;
    }
  }

  return 0;
}