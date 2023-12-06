#include "state_hasher.h"

std::size_t StateHasher::operator()(const StatePtr s) const
{
  std::size_t hashValue = 0;

  hash_combine(hashValue, s->getIsFinal());
  hash_combine(hashValue, s->getId());

  for (const auto &trans : s->getTransitions())
  {
    hash_combine(hashValue, trans.first);
    hash_combine(hashValue, trans.second.second);
    //hash_combine(hashValue, trans.second.second->getId());
  }

  return hashValue;
}

template <class T>
void StateHasher::hash_combine(std::size_t &seed, const T &value) const
{
  std::hash<T> hasher;
  seed ^= hasher(value) + 0x9e3779b9;
}