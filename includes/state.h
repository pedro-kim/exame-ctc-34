#include <unordered_map>
#include <string>

class State {
public:
    State();

    bool getIsFinal();
    void setIsFinal(bool);
    
    void transition(char);
    void addTransition(char, State);

    std::unordered_map<std::string, char> stateOutput();
    
private:

    bool isFinal_;

};