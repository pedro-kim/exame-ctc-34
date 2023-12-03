#include "transducer.h"

StatePtr Transducer::findMinimized(StatePtr s) {
    StatePtr r = nullptr;

    if(states_.find(*s) != states_.end()) {
        auto it = states_.find(*s);
        r = it->first;
    }

    else {
        std::make_shared<State>(r);
        r->setIsFinal(s->getIsFinal());
        r->setStateOuput(s->getStateOutput());
        
        for(auto& pair : s->transitions_) {
            r->transitions_[pair.first] = pair.second;
        }

        states_[*r] = numberOfStates++;
        numberOfEdges += r->transitions_.size();
    }

    return r;
}

