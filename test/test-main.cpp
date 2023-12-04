#include <unistd.h>
#include <iostream>
#include "../includes/transducer.h"
#include "../includes/trie.h"
#include "../includes/levenshtein.h"

int transducer_test() {
  std::cout << "Hello World! FST" << std::endl;
  std::string currentDirectory = get_current_dir_name();

  Transducer t;

  std::cout << "Before" << std::endl;
  Transducer::make(currentDirectory + "/assets/data/test.txt", t);
  std::cout << "After" << std::endl;

  std::cout << "Number of words: " << t.getNumberOfWords() << std::endl;
  std::cout << "Number of states: " << t.getNumberOfStates() << std::endl;
  std::cout << "Number of edges: " << t.getNumberOfEdges() << std::endl;

  std::string dotPath = currentDirectory + "/assets/graphs/test_fst.dot";
  std::string pngPath = currentDirectory + "/assets/images/test_fst.png";

  t.printTransducer(dotPath);

  std::string command = "dot -Tpng " + dotPath + " -o " + pngPath;
  system(command.c_str());
  return 0;
}

int trie_test() {

  std::cout << "Hello World! Trie" << std::endl;

  return  0;
}

int levenshtein_test() {
  std::cout << "Hello World! Levenshtein" << std::endl;
  return 0;
}


int main() {
  std::cout << "Hello World! Main" << std::endl;

  bool valid_test = false;

  while (!valid_test) {
    std::string test = "fst";
    std::cout << "Choose a test (fst, trie, lev): ";
    // std::cin >> test;

    if (test == "fst") {
      transducer_test();
      valid_test = true;
    }
    else if (test == "trie") {
      trie_test();
      valid_test = true;
    }
    else if (test == "lev") {
      levenshtein_test();
      valid_test = true;
    }
    else {
      std::cout << "Invalid test. Try again." << std::endl;
    }
  }

  return 0;
}