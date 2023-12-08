#include "../includes/transducer.h"
#include <iostream>
#include <fstream>

Transducer::Transducer() : numberOfEdges_(0), numberOfStates_(0), numberOfWords_(0)
{
  for (auto& s : tempStates_)
    s = std::make_shared<State>();
}

void Transducer::make(const std::string& filePath, Transducer& t)
{
  std::ifstream inputFile(filePath);

  if (!inputFile.is_open())
  {
    std::cerr << "Error opening file: " << filePath << std::endl;
    return;
  }

  t.words_.clear();

  std::string previousWord, currentWord, currentOutput, tempOutput, wordSuffix, commonPrefix;
  previousWord = "";

  unsigned int prefixLengthPlus1, aux;

  while (std::getline(inputFile, currentWord))
  {
    currentOutput = std::to_string(t.getNumberOfWords());
    //std::cout << t.getNumberOfWords() << std::endl;
    t.increaseNumberOfWords(); // TODO: switch number of words
    prefixLengthPlus1 = 0;

    while ((prefixLengthPlus1 < currentWord.size()) &&
      (prefixLengthPlus1 < previousWord.size()) &&
      (previousWord[prefixLengthPlus1] == currentWord[prefixLengthPlus1]))
      prefixLengthPlus1++;

    // int prefixLengthPlus1 = i;

    for (int i = previousWord.size(); i > prefixLengthPlus1; i--)
      t.tempStates_[i - 1]->setTransition(previousWord[i - 1], t.findMinimized(t.tempStates_[i]));

    for (int i = prefixLengthPlus1; i < currentWord.size(); i++)
    {
      t.tempStates_[i + 1]->cleanTransitions();
      t.tempStates_[i]->setTransition(currentWord[i], t.tempStates_[i + 1]);
    }

    t.tempStates_[currentWord.size()]->setIsFinal(true);

    for (int j = 0; j < prefixLengthPlus1; j++)
    {
      tempOutput = t.tempStates_[j]->getOutput(currentWord[j]);
      aux = 0;

      while (aux < currentOutput.size() && aux < tempOutput.size() && currentOutput[aux] == tempOutput[aux])
        aux++;

      commonPrefix = tempOutput.substr(0, aux);
      wordSuffix = tempOutput.substr(aux, tempOutput.size());
      currentOutput = currentOutput.substr(aux, currentOutput.size());

      t.tempStates_[j]->setOutput(currentWord[j], commonPrefix);

      for (auto& pair : t.tempStates_[j + 1]->getTransitions())
        t.tempStates_[j + 1]->setOutput(pair.first, (wordSuffix + t.tempStates_[j + 1]->getOutput(pair.first)));

      if (t.tempStates_[j + 1]->getIsFinal())
        t.tempStates_[j + 1]->setStateOutput(wordSuffix + t.tempStates_[j + 1]->getStateOutput());
    }

    t.tempStates_[prefixLengthPlus1]->setOutput(currentWord[prefixLengthPlus1], currentOutput);
    previousWord = currentWord;
  }

  inputFile.close();

  for (int i = previousWord.size(); i > 0; i--)
    t.tempStates_[i - 1]->setTransition(previousWord[i - 1], t.findMinimized(t.tempStates_[i]));

  t.setInitialState(t.findMinimized(t.tempStates_[0]));

  return;
}

StatePtr Transducer::getInitialState()
{
  return initialState_;
}
void Transducer::setInitialState(StatePtr initial)
{
  initialState_ = initial;
}

unsigned int Transducer::getNumberOfStates()
{
  return numberOfStates_;
}
void Transducer::increaseNumberOfStates()
{
  numberOfStates_++;
}

unsigned int Transducer::getNumberOfEdges()
{
  return numberOfEdges_;
}
void Transducer::increaseNumberOfEdges()
{
  numberOfEdges_++;
}

unsigned int Transducer::getNumberOfWords()
{
  return numberOfWords_;
}
void Transducer::increaseNumberOfWords()
{
  numberOfWords_++;
}

StatePtr Transducer::findMinimized(StatePtr s)
{
  // TODO: Verify findMinimized
  StatePtr r = nullptr;

  if (states_.find(s) != states_.end())
  {
    auto it = states_.find(s);
    r = it->first;
  }

  else
  {
    r = std::make_shared<State>();
    r->setIsFinal(s->getIsFinal());
    r->setStateOutput(s->getStateOutput());

    for (auto& pair : s->getTransitions())
    {
      r->transitions_[pair.first] = pair.second;
      // r->setTransition(pair.first, pair.second.second);
      // r->getTransitions()[pair.first] = pair.second;
    }

    states_[r] = numberOfStates_++;
    numberOfEdges_ += r->getTransitions().size();
  }

  return r;
}

void Transducer::printTransducer(const std::string& printerFolder)
{
  // TODO: Verify printer
  std::ofstream graphFile(printerFolder);

  graphFile << "digraph G {\n";
  graphFile << "rankdir=LR;\n";
  graphFile << "charset=\"utf8\";\n";
  graphFile << "node [shape=circle];\n";
  graphFile << "start [shape=box style=filled fillcolor=red];\n";
  graphFile << "start -> q" << states_[initialState_] << ";\n";

  for (auto& state : states_) {
    if (state.first->getStateOutput() != "") {
      graphFile << "\tq" << state.second << " [label=\"q" << state.second << " / " << state.first->getStateOutput() << "\"];\n";
    }

    else {
      graphFile << "\tq" << state.second << " [label=\"q" << state.second << "\"];\n";
    }
    if (state.first->getIsFinal()) {
      graphFile << "\tq" << state.second << " [shape=doublecircle];\n";
      graphFile << "\tq" << state.second << " [style=filled fillcolor=green];\n";
    }
  }

  for (auto& state : states_) {
    for (auto& transition : state.first->getTransitions()) {

      if (transition.second.first != "") {
        graphFile << "\tq" << state.second << " -> q" << states_[transition.second.second] << " [label=\"" << transition.first << " / " << transition.second.first << "\"];\n";
      }
      else {
        graphFile << "\tq" << state.second << " -> q" << states_[transition.second.second] << " [label=\"" << transition.first << "\"];\n";
      }
    }
  }

  graphFile << "}\n";
}

std::vector<std::string> Transducer::find_suggestions(const std::string& prefix) {
  std::vector<bool> visited(numberOfStates_, false);
  std::vector<std::string> suggestions;
  StatePtr current = initialState_;
  std::string currentSuggestion = "";

  if (prefix.size() == 0) {
    return suggestions;
  }

  for (int i = 0; i < prefix.size(); i++) {
    if (current->transitions_.find(prefix[i]) == current->transitions_.end()) {
      return suggestions;
    }
    current = current->transitions_[prefix[i]].second;
  }

  currentSuggestion = prefix;
  dfs_recursive(current, visited, suggestions, currentSuggestion);

  // for (int i = 0; i < suggestions.size(); i++){
  //   std::cout << suggestions[i] << std::endl;
  // }
  return suggestions;
}

void Transducer::dfs_recursive(StatePtr current, std::vector<bool>& visited, std::vector<std::string>& suggestions, std::string& currentSuggestion) {

  if (!current) {
    return;
  }
  // if (visited[current->getId()] == true) {
  //     return;
  // }

  // // Process the current state (optional)
  // std::cout << "Visiting State " << current->getId() << std::endl;


  if (current->getIsFinal() == true) {
    suggestions.push_back(currentSuggestion);
  }

  visited[current->getId()] = true;

  for (const auto& pair : current->transitions_) {
    StatePtr next = pair.second.second;
    currentSuggestion.push_back(pair.first);
    dfs_recursive(next, visited, suggestions, currentSuggestion);
  }

  currentSuggestion.pop_back();
}

void Transducer::estimateMemoryUsage() {
  std::cout << "Espaço de memória de um nó da árvore: " << sizeof(StatePtr) << " bytes" << std::endl;
  std::cout << "Espaço de memória total ocupado: " << numberOfStates_ * sizeof(StatePtr) << " bytes" << std::endl;
}