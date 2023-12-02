#include "state.h"
#include <vector>

class Transducer {
public:
    Transducer();
    ~Transducer();

    void printTransducer(const std::string& printerFolder);
    

    StatePtr initialState;

    unsigned int numberOfStates = 0;
    unsigned int numberOfEdges = 0;
    unsigned int numberOfWords = 0;

    std::vector<std::string> words;
private:

};