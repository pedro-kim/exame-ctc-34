#include "transducer.h"

#include <iostream>
#include <fstream>

Transducer::Transducer() : numberOfEdges_(0), numberOfStates_(0), numberOfWords_(0)
{
  for (auto &s : tempStates_)
    s = std::make_shared<State>();
}

void Transducer::make(const std::string &filePath, Transducer &t)
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
    t.increaseNumberOfWords();
    currentOutput = std::to_string(t.getNumberOfWords());
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

      for (auto &pair : t.tempStates_[j + 1]->getTransitions())
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
  StatePtr r = nullptr;

  if (states_.find(s) != states_.end())
  {
    auto it = states_.find(s);
    r = it->first;
  }

  else
  {
    r->setIsFinal(s->getIsFinal());
    r->setStateOutput(s->getStateOutput());

    for (auto &pair : s->getTransitions())
    {
      r->getTransitions()[pair.first] = pair.second;
    }

    states_[r] = numberOfStates_++;
    numberOfEdges_ += r->getTransitions().size();
  }

  return r;
}
