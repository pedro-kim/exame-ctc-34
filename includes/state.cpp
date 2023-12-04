#include "state.h"
using StatePtr = std::shared_ptr<State>;

State::State() : isFinal_(false), output_(""), id_(idCounter++) {}

bool operator==(const State &left, const State &right)
{
  if (left.isFinal_ != right.isFinal_)
    return false;
  if (left.output_ != right.output_)
    return false;
  if (left.transitions_.size() != right.transitions_.size())
    return false;

  for (auto &pair : right.transitions_)
  {
    auto left_it = left.transitions_.find(pair.first);

    if (left_it == left.transitions_.end())
      return false;
    if (pair.second.first != left_it->second.first)
      return false;
    if (pair.second.second != left_it->second.second)
      return false;
  }

  return true;
}

bool State::getIsFinal()
{
  return this->isFinal_;
}

void State::setIsFinal(bool b)
{
  this->isFinal_ = b;
}

unsigned int State::getId()
{
  return id_;
}

StatePtr State::copyOfState()
{
  return std::make_shared<State>(*this);
}

// StatePtr &State::transition(char c)
// {
//   if (this->transitions_.find(c) == this->transitions_.end())
//     return;
// }

void State::setTransition(char c, StatePtr toState)
{
  if (transitions_.find(c) == transitions_.end())
    transitions_[c] = std::make_pair("", toState);
  else
    transitions_[c].second = toState;
}

void State::cleanTransitions()
{
  transitions_.clear();
  isFinal_ = false;
  output_ = "";
}

std::unordered_map<char, std::pair<std::string, StatePtr>> State::getTransitions()
{
  return transitions_;
}

std::string State::getStateOutput()
{
  return output_;
}

void State::setStateOutput(std::string s)
{
  output_ = s;
}

std::string State::getOutput(char c)
{
  return transitions_[c].first;
}

void State::setOutput(char c, std::string s)
{
  transitions_[c].first = s;
}