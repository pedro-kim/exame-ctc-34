#include "state.h"
#include "state_hasher.h"
#include "state_equal.h"
#include "auxiliar.h"

#include <vector>

class Transducer {
public:
    Transducer() = default;
    ~Transducer() = default;

    void printTransducer(const std::string& printerFolder);
    void generate(const std::string& filePath);

    StatePtr getInitialState();

    unsigned int getNumberOfStates();
    void setNumberOfStates(unsigned int);

    unsigned int getNumberOfEdges();
    void setNumberOfEdges(unsigned int);

    unsigned int getNumberOfWords();
    void setNumberOfWords(unsigned int);

    std::vector<std::string> words;

private:
    StatePtr findMinimized(StatePtr);

    std::unordered_map<std::string, StatePtr, StateHasher, StateEqual> states_;
    StatePtr tempStates[MAX_WORD_SIZE];
    StatePtr initialState;

    unsigned int numberOfStates = 0;
    unsigned int numberOfEdges = 0;
    unsigned int numberOfWords = 0;
};