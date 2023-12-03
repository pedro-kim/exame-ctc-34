#include "state.h"
#include "state_hasher.h"
#include "auxiliar.h"

#include <vector>

class Transducer {
public:
    Transducer() = default;
    ~Transducer();

    void printTransducer(const std::string& printerFolder);
    void generate(const std::string& filePath);

    StatePtr initialState;

    unsigned int numberOfStates = 0;
    unsigned int numberOfEdges = 0;
    unsigned int numberOfWords = 0;

    std::vector<std::string> words;

private:
    StatePtr findMinimized(StatePtr);

    std::unordered_map<std::string, StatePtr, StateHasher> states_;
    StatePtr tempStates[MAX_WORD_SIZE];

};