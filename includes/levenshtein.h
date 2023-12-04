#ifndef LEVENSHTEIN
#define LEVENSHTEIN

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <map>

using lev_state = std::vector<int>;

class LevenshteinDFA {

    public:

        LevenshteinDFA(const std::string& input, int distance){this->input = input;this->distance = distance;};
        ~LevenshteinDFA(void);
        void generateDFA(void);

    private:

        std::string input;
        int distance;

        lev_state start(void);
        lev_state step(const lev_state& cur_state, char c);
        bool is_match(const lev_state cur_state);
        bool can_match(const lev_state cur_state);
        std::unordered_set<char> transitions(const lev_state cur_state);
        int explore(lev_state& cur_state,
                    std::map<lev_state, int>& states_map,
                    unsigned int& counter,
                    std::vector<int>& matching,
                    std::vector<std::tuple<int, int, char>>& transitionsStates);
        

};

#endif