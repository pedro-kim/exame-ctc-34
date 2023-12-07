#include "state.h"
#include "state_hasher.h"
#include "state_equal.h"

#include <vector>

#define MAX_WORD_SIZE 30

class Transducer
{
public:
  Transducer();

  static void make(const std::string &, Transducer &);

  void printTransducer(const std::string &printerFolder);

  StatePtr getInitialState();
  void setInitialState(StatePtr);

  unsigned int getNumberOfStates();
  void increaseNumberOfStates();

  unsigned int getNumberOfEdges();
  void increaseNumberOfEdges();

  unsigned int getNumberOfWords();
  void increaseNumberOfWords();

  std::vector<std::string> find_suggestions(const std::string &);

private:
  StatePtr findMinimized(StatePtr);

  std::unordered_map<StatePtr, std::size_t, StateHasher, StateEqual> states_;
  StatePtr tempStates_[MAX_WORD_SIZE];
  StatePtr initialState_;

  std::vector<std::string> words_;

  void dfs_recursive(StatePtr current,std::vector<bool>& visited, std::vector<std::string>& suggestions, std::string& currentSuggestion);

  unsigned int numberOfStates_;
  unsigned int numberOfEdges_;
  unsigned int numberOfWords_;
};