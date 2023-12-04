#ifndef STATE_H
#define STATE_H

#include <memory>
#include <map>
#include <unordered_map>
#include <string>

static unsigned int idCounter = 0;

class State : public std::enable_shared_from_this<State>
{

public:
  State();

  friend bool operator==(const State &, const State &); // Operation overload for comparing two states

  bool getIsFinal();     // Returns if this is final
  void setIsFinal(bool); // Sets if this is final

  std::shared_ptr<State> &transition(char);         // Returns the pointer to the state which results from the transition by char
  void setTransition(char, std::shared_ptr<State>); // Sets a specific transition by a char to a state pointer
  void cleanTransitions();                          // Cleans all transitions of this
  std::map<char, std::pair<std::string, std::shared_ptr<State>>> getTransitions();

  std::shared_ptr<State> copyOfState(); // Returns a copy of this

  unsigned int getId();

  std::string getStateOutput();
  void setStateOutput(std::string);

  std::string getOutput(char);       // Returns string from transition by char
  void setOutput(char, std::string); // Sets the string output from transition by char

private:
  bool isFinal_;
  std::string output_;
  const unsigned int id_;

  std::map<char, std::pair<std::string, std::shared_ptr<State>>> transitions_;
};

#endif