#include "state_equal.h"

bool StateEqual::operator()(const StatePtr left, const StatePtr right) const
{
  if (left->getIsFinal() != right->getIsFinal())
    return false;

  if (left->getStateOutput() != right->getStateOutput())
    return false;

  if (left->getTransitions().size() != right->getTransitions().size())
    return false;

  for (const auto &pair : right->getTransitions())
  {
    auto left_it = left->getTransitions().find(pair.first);

    if (left_it == left->getTransitions().end())
      return false;
    if (pair.second.first != left_it->second.first)
      return false;
    if (pair.second.second != left_it->second.second)
      return false;
  }

  return true;
}