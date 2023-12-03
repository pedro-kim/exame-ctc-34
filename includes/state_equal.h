#ifndef STATE_EQUAL
#define STATE_EQUAL

#include "state.h"

struct StateEqual {
    bool operator()(const StatePtr, const StatePtr) const;
};

#endif