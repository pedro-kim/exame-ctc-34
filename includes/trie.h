#include <unordered_map>
#include <string>
#include <vector>
#include "state.h"
#include "state_hasher.h"
#include "state_equal.h"

#define MAX_WORD_SIZE 30

using StatePtr = std::shared_ptr<State>;
class Trie
{
public:
  Trie();

  ~Trie();

  static void make(const std::string &filePath, Trie &T);

  void printTrie();

  StatePtr getInitialState();

  unsigned int getNumberOfStates();
  void setNumberOfStates(unsigned int);

  unsigned int getNumberOfEdges();
  void setNumberOfEdges(unsigned int);

  unsigned int getNumberOfWords();
  void setNumberOfWords(unsigned int);

  std::vector<std::string> words;

private:
  StatePtr findMinimized(StatePtr);

  std::unordered_map<std::string, StatePtr, StateHasher, StateEqual> states_;
  StatePtr tempStates[MAX_WORD_SIZE];
  StatePtr initialState;

  unsigned int numberOfStates = 0;
  unsigned int numberOfEdges = 0;
  unsigned int numberOfWords = 0;
};