#include "state.h"

State::State() : isFinal_(false), output_(""), id_(idCounter++) {}

bool operator==(const State& lhs, const State& rhs) {

}

bool State::getIsFinal() {
    return this->isFinal_;
}

void State::setIsFinal(bool exp) {
    this->isFinal_ = exp;
}

StatePtr State::copyOfState() {
    return std::make_shared<State>(*this);
}

StatePtr& State::transition(char c) {
    if(this->transitions_.find(c) == this->transitions_.end())
        return;
}