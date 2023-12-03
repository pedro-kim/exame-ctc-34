#ifndef STATE_H
#define STATE_H

#include <memory>
#include <unordered_map>
#include <string>

using StatePtr = std::shared_ptr<State>;

static unsigned int idCounter = 0;

class State : public std::enable_shared_from_this<State> {
public:
    State();

    // static StatePtr make();

    // static StatePtr make(bool, std::string);

    friend bool operator==(const State&, const State&); // Operation overload for comparing two states

    bool getIsFinal(); // Returns if this is final
    void setIsFinal(bool); // Sets if this is final

    unsigned int getId();

    StatePtr copyOfState(); // Returns a copy of this
    
    StatePtr& transition(char); // Returns the pointer to the state which results from the transition by char
    void setTransition(char, std::pair<std::string, StatePtr&>); // Sets a specific transition by a char to a state pointer
    void cleanTransition(); // Cleans all transitions of this
    std::unordered_map<char, std::pair<std::string, StatePtr>> getTransitions();

    std::string getStateOutput();
    void setStateOutput(std::string);

    std::string getOutput(char); // Returns string from transition by char
    void setOutput(char, std::string); // Sets the string output from transition by char
    
private:
    bool isFinal_;
    std::string output_;
    const unsigned int id_;

    std::unordered_map<char, std::pair<std::string, StatePtr>> transitions_;
};

#endif