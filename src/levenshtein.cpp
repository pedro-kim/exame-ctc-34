#include <levenshtein.h>

// inicializa o primeiro estado de Levenshtein
lev_state LevenshteinDFA::start(void) {

    lev_state v(input.size() + 1);

    for (size_t i = 0; i <= input.size(); ++i) v[i] = i;
    
    return v;
}

// Retorna um novo estado (linha da matriz de PD) a partir do estado atual e do novo caracter lido
lev_state LevenshteinDFA::step(const lev_state &cur_state, char c) {

    lev_state new_state;
    new_state.push_back(cur_state[0] + 1);

    for (size_t i = 0; i < cur_state.size() - 1; ++i) {
        int cost = (input[i] == c) ? 0 : 1;
        new_state.push_back(std::min({new_state[i] + 1, cur_state[i] + cost, cur_state[i + 1] + 1}));
    }
    
    for (auto &x : new_state) {
        x = std::min(x, distance + 1); 
    }

    return new_state;
}

// Verifica se a "string do estado atual" está a uma distância de Levenshstein aceitável do input
bool LevenshteinDFA::is_match(const lev_state cur_state) {return cur_state.back() <= distance; }

// Verifica se é possível que a "string do estado atual" esteja a uma distância de Levenshstein aceitável do input
bool LevenshteinDFA::can_match(const lev_state cur_state) {return *std::min_element(cur_state.begin(), cur_state.end()) <= distance; }

// Retorna um set com os caracteres que podem ser lidos a partir do estado atual
std::unordered_set<char> LevenshteinDFA::transitions(const lev_state cur_state) {

    std::unordered_set<char> result;
    
    for (size_t i = 0; i < input.size(); ++i) {
        if (cur_state[i] <= distance) {
            result.insert(input[i]);
        }
    }

    return result;
}

// Função recursiva que explora o DFA em um DFS e retorna a numeração dos estados a partir da ordem de visita
// states_map deve ser inicializado com todos os estados possíveis
// transitions_vector armazena todas as transilçoes (estado incial, estado final, caracter lido)
int LevenshteinDFA::explore(lev_state& cur_state,
                            std::map<lev_state, int>& states_map,
                            unsigned int& counter,
                            std::vector<int>& matching,
                            std::vector<std::tuple<int, int, char>>& transitions_vector) {

    if (states_map.find(cur_state) != states_map.end()) {
        return states_map[cur_state];
    }

    states_map[cur_state] = counter++;

    if (is_match(cur_state)) {
        matching.push_back(states_map[cur_state]);
    }

    std::unordered_set<char> transitions_set = transitions(cur_state);
    transitions_set.insert('*');

    for (char c : transitions_set) {
        lev_state next_state(step(cur_state, c));
        int exp = explore(next_state, states_map, counter, matching, transitions_vector);
        transitions_vector.push_back(std::make_tuple(states_map[cur_state], exp, c));
    }

    return states_map[cur_state];
}

// Gera o DFA de Levenshtein
void LevenshteinDFA::generateDFA() {

    std::map<lev_state, int> states_map;
    unsigned int counter = 0;
    std::vector<int> matching;

    lev_state initial_state(start());
    explore(initial_state, states_map, counter, matching, transitions_vector);

    dfa_vector.resize(counter);

    // cuidado com o id counter
    for (int i = 0; i < counter; ++i) {
        dfa_vector[i] = std::make_shared<State>();
        dfa_vector[i]->cleanTransition();
    }

    for (auto &trans : transitions_vector) {
        int start, end;
        char label;

        start = std::get<0>(trans);
        end = std::get<1>(trans);
        label = std::get<2>(trans);
        dfa_vector[start]->transitions[label] = dfa_vector[end];
    }

    std::cout << "digraph G {\n";
    std::cout << "    rankdir=LR;\n";
    std::cout << "    size=\"8,5\"\n";
    std::cout << "    node [shape = doublecircle];\n";

    for (auto &x : matching) {
        std::cout << "    " << x << ";\n";
    }

    std::cout << "    node [shape = circle];\n";

    for (auto &x : transitions_vector) {
        std::cout << "    " << std::get<0>(x) << " -> " << std::get<1>(x) << " [ label = \"" << std::get<2>(x) << "\" ];\n";
    }

    std::cout << "}\n";
}