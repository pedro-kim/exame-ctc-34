#include <iostream>
#include <vector>
#include "../includes/transducer.h"
#include "../includes/rbtree.h"
#include <ncurses.h>

int main()
{

  initscr();            // Initializes curses
  keypad(stdscr, TRUE); // Enable special keys
  cbreak();             // Disable line buffering

  // Main variables
  char ch;
  std::string input;
  int algorithmChoice;
  int levenshteinChoice;
  int levenshteinDistance;

  // Main Automata
  Transducer t;
  RBTree rbt;

  // Get user's choose of algorithm
  printw("Choose an autocomplete algorithm: \n 1 for MAST;\n 2 for Red Black Tree.\n");
  refresh();
  scanw("%d", &algorithmChoice);

  while (algorithmChoice != 1 && algorithmChoice != 2)
  {
    printw("Invalid algorithm choice. Please choose 1 or 2.\n");
    refresh();
    scanw("%d", &algorithmChoice);
  }

  // Get user's decision whether to use Levenshtein or not
  printw("Do you want to use the Levenshtein automata? \n 1 for Yes;\n 0 for No.\n");
  refresh();
  scanw("%d", &levenshteinChoice);

  while (levenshteinChoice != 1 && levenshteinChoice != 0)
  {
    printw("Invalid choice. Please type 1 or 0.\n");
    refresh();
    scanw("%d", &levenshteinChoice);
  }

  if (levenshteinChoice)
  {
    printw("Choose Levenshtein distance (0, 1, or 2): ");
    refresh();
    scanw("%d", &levenshteinDistance);

    while (levenshteinDistance != 0 && levenshteinDistance != 1 && levenshteinDistance != 2)
    {
      printw("Unavailable Levenshtein distance, choose 0, 1 or 2: ");
      refresh();
      scanw("%d", &levenshteinDistance);
    }
  }

  clear();
  refresh();
  printw("Enter your word (press Esc to exit): ");
  refresh();

  while ((ch = getch()) != 27 /* ASCII value for Esc key*/)
  {
    refresh();
    if (ch == KEY_BACKSPACE)
    {
      input = input.substr(0, input.size() - 1);
    }
    else
    {
      input += ch;
    }
    printw("%s\n", input.c_str());
    printw("Autocomplete suggestions:\n");

    std::vector<std::string> suggestions;
    if (algorithmChoice == 1)
    {
      printw("MAST test.\n");
      refresh();
    }
    else if (algorithmChoice == 2)
    {
      suggestions = rbt.find_prefix(input);
      for (const auto &suggestion : suggestions)
      {
        printw("%s\n", suggestion.c_str());
        refresh();
      }
    }

    if (levenshteinChoice)
    {
      std::vector<std::string> filteredSuggestions;
      printw("Levenshtein test.\n");
      refresh();
    }

    // printw("Input: %s\n", input.c_str());
    // clear();
    refresh();
  }

  endwin();

  return 0;
}