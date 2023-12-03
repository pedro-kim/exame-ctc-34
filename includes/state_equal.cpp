#include "state_equal.h"

bool StateEqual::operator()(const StatePtr left, const StatePtr right) const {
    if(left->isFinal_ != right->isFinal_)
        return false;

    if(left->output_ != right->ouput_)
        return false;

    if(left->transitions_.size() != right->transitions_.size())
        return false;


    for(const auto& pair : right->transitions_) {
        auto left_it = left->transitions_.find(pair.first);

        if(left_it == left->transitions_.end())
            return false;
        if(pair.second.first != left_it->second.first)
            return false;
        if(pair.second.second != left_it->second.second)
            return false;        
    }

    return true;
}