#ifndef STATE_EQUAL
#define STATE_EQUAL

#include "state.h"
using StatePtr = std::shared_ptr<State>;

struct StateEqual
{
  bool operator()(const StatePtr, const StatePtr) const;
};

#endif