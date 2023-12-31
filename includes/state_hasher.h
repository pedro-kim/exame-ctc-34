#ifndef STATE_HASHER
#define STATE_HASHER

#include "state.h"
using StatePtr = std::shared_ptr<State>;

struct StateHasher
{
  std::size_t operator()(const StatePtr) const;

private:
  template <class T>
  void hash_combine(std::size_t &, const T &) const;
};

#endif