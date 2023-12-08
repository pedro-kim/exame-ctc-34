#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include "../includes/transducer.h"
#include "../includes/rbtree.h"
#include "../includes/levenshtein.h"
#include <ncurses.h>

int main()
{

  initscr();            // Initializes curses
  raw();
  noecho();
  keypad(stdscr, TRUE); // Enable special keys
  cbreak();             // Disable line buffering

  // Main variables
  char ch;
  std::string input;
  int algorithmChoice;
  int levenshteinChoice;
  int levenshteinDistance;
  std::string currentDirectory = get_current_dir_name();

  // Main Automata
  Transducer t;
  RBTree rbt;

  // Additional information variables
  std::string chosenAlgorithm;
  std::string levenshteinEnabled;
  long storingTime;
  std::string memoryUsedNode;
  std::string memoryUsedData;
  int numberOfWords;

  // Get user's choose of algorithm
  printw("Choose an autocomplete algorithm: \n 1 for MAST;\n 2 for Red Black Tree.\n");
  refresh();
  algorithmChoice = getch() - '0';
  printw("%d\n", algorithmChoice);
  refresh();

  while (algorithmChoice != 1 && algorithmChoice != 2)
  {
    printw("Invalid algorithm choice. Please choose 1 or 2.\n");
    refresh();
    algorithmChoice = getch() - '0';
    printw("%d\n", algorithmChoice);
    refresh();
  }

  auto start = std::chrono::high_resolution_clock::now();
  switch (algorithmChoice)
  {
  case 1:
    Transducer::make(currentDirectory + "/assets/data/american-english-sorted.txt", t);
    numberOfWords = t.getNumberOfWords();
    chosenAlgorithm = "FST";
    break;
  case 2:
    rbt.make(currentDirectory + "/assets/data/american-english-sorted.txt");
    numberOfWords = rbt.getNumberOfNodes();
    chosenAlgorithm = "RBT";
    break;
  default:
    break;
  }
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  storingTime = duration.count();
  printw("\tTime taken by function: %ld milliseconds.\n", storingTime);

  // Estimate memory usage
  std::pair<std::string, std::string> memory;
  if (algorithmChoice == 1)
  {
    t.estimateMemoryUsage(memory);
  }

  if (algorithmChoice == 2)
  {
    rbt.estimateMemoryUsage(memory);
  }

  memoryUsedNode = memory.first;
  memoryUsedData = memory.second;

  printw("\tMemory used by one node: %s bytes\n", memoryUsedNode.c_str());
  printw("\tMemory used by all nodes: %s bytes\n", memoryUsedData.c_str());

  // Get user's decision whether to use Levenshtein or not
  printw("\nDo you want to use the Levenshtein automata? \n 1 for Yes;\n 0 for No.\n");
  refresh();
  levenshteinChoice = getch() - '0';
  printw("%d\n", levenshteinChoice);
  refresh();

  while (levenshteinChoice != 1 && levenshteinChoice != 0)
  {
    printw("Invalid choice. Please type 1 or 0.\n");
    refresh();
    levenshteinChoice = getch() - '0';
    printw("%d\n", levenshteinChoice);
    refresh();
  }

  levenshteinEnabled = (levenshteinChoice == 1) ? "TRUE" : "FALSE";

  if (levenshteinChoice)
  {
    printw("Choose Levenshtein distance (0, 1, or 2): ");
    refresh();
    levenshteinDistance = getch() - '0';
    printw("%d\n", levenshteinDistance);
    refresh();

    while (levenshteinDistance != 0 && levenshteinDistance != 1 && levenshteinDistance != 2)
    {
      printw("Unavailable Levenshtein distance, choose 0, 1, or 2: ");
      refresh();
      levenshteinDistance = getch() - '0';
      printw("%d\n", levenshteinDistance);
      refresh();
    }
  }

  clear();
  refresh();
  printw("Enter your word (press Esc to exit): ");
  refresh();

  std::pair<int, int> cursorToInput = std::make_pair(0, 0);

  // Dynamic Part
  while (true)
  {
    int ch = getch();
    if (ch == 27 || ch == '\n')
      break;

    else if (ch == KEY_BACKSPACE)
    {
      if (!input.empty())
      {
        input.pop_back();
      }
    }

    if (!std::isdigit(ch) && ch != ' ' && ch != KEY_BACKSPACE && ch != 27 && ch != '\n' && input.size() < MAX_WORD_SIZE)
    {
      input += ch;
    }
    refresh();
    clear();

    // Print additional information at the bottom right corner
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    attron(A_BOLD);
    mvprintw(maxY - 7, maxX - 30, "Information:");
    attroff(A_BOLD);
    mvprintw(maxY - 6, maxX - 30, "\tChoosed Algorithm: %s", chosenAlgorithm.c_str());
    mvprintw(maxY - 5, maxX - 30, "\tLevenshtein Enabled: %s", levenshteinEnabled.c_str());
    mvprintw(maxY - 4, maxX - 30, "\tStoring time: %ld ms", storingTime);
    mvprintw(maxY - 3, maxX - 30, "\tMemory Used(Node): %s", memoryUsedNode.c_str());
    mvprintw(maxY - 2, maxX - 30, "\tMemory Used(Data): %s", memoryUsedData.c_str());
    mvprintw(maxY - 1, maxX - 30, "\tNumber of words: %d", numberOfWords);

    refresh();

    // Print the constant line
    mvprintw(0, 0, "Enter your word (press Esc to exit): ");
    refresh();

    printw("%s", input.c_str());
    refresh();
    cursorToInput.first = 37 + input.size();
    cursorToInput.second = 0;

    // Print other information based on user's choices
    std::vector<std::string> suggestions;
    std::vector<std::string> levSuggestions;
    if (algorithmChoice == 1)
    {
      if (!levenshteinChoice)
      {
        suggestions = t.find_suggestions(input);
      }
      else
      {
        LevenshteinDFA lev(input, levenshteinDistance);
        lev.generateDFA();
        suggestions = t.find_suggestions(input);
        levSuggestions = lev.find_suggestions(suggestions);
      }
    }
    else if (algorithmChoice == 2)
    {
      suggestions = rbt.find_prefix(input);
    }

    // Print the constant line
    printw("\nAutocomplete suggestions:\n");
    int i = 0;
    if (levenshteinChoice)
    {
      for (const auto& suggestion : levSuggestions)
      {
        printw("\t%s\n", suggestion.c_str());
        refresh();
        i++;
        if (i == 20)
          break;
      }
    }
    else
    {
      printw("\n %ld\n", suggestions.size());
      for (const auto& suggestion : suggestions)
      {
        printw("\t%s\n", suggestion.c_str());
        refresh();
        i++;
        if (i == 20)
          break;
      }
    }
    refresh();
    cursorToInput.first = 37 + input.size();
    cursorToInput.second = 0;
    move(cursorToInput.second, cursorToInput.first);

    
    refresh();
  }

  endwin();

  return 0;
}