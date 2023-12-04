#include "state.h"
using StatePtr = std::shared_ptr<State>;

State::State() : isFinal_(false), output_(""), id_(idCounter++) {}

bool operator==(const State& lhs, const State& rhs) {

}

bool State::getIsFinal() {
    return this->isFinal_;
}

void State::setIsFinal(bool b) {
    this->isFinal_ = b;
}

StatePtr State::copyOfState() {
    return std::make_shared<State>(*this);
}

StatePtr& State::transition(char c) {
    if(this->transitions_.find(c) == this->transitions_.end())
        return;
}

void State::setTransition(char c, std::pair<std::string, StatePtr&> pair) {

}

void State::cleanTransition() {

}

std::unordered_map<char, std::pair<std::string, StatePtr>> State::getTransitions() {

}

std::string State::getStateOutput() {
    return this->output_;
}

void State::setStateOutput(std::string s) {
    this->output_ = s;
}