#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
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
  std::string currentDirectory = get_current_dir_name();

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

  auto start = std::chrono::high_resolution_clock::now();
  switch (algorithmChoice)
  {
  case 1:
    Transducer::make(currentDirectory + "/assets/data/american-english-sorted.txt", t);
    break;
  case 2:
    rbt.make(currentDirectory + "/assets/data/american-english-sorted.txt");
  default:
    break;
  }
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  printw("Time taken by function: %d milliseconds.\n", duration.count());

  // Get user's decision whether to use Levenshtein or not
  printw("\nDo you want to use the Levenshtein automata? \n 1 for Yes;\n 0 for No.\n");
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

  while (true)
  {
    char ch = getch();
    if (ch == 27 || ch == '\n')
      break;

    else if (input.size() > 0 && ch == 127 /* ASCII value for Backspace key*/)
    {
      input.pop_back();
    }
    if (!std::isdigit(ch) && ch != ' ' && ch != 127 && ch != 27 && ch != '\n' && input.size() < MAX_WORD_SIZE)
    {
      input += ch;
    }
    refresh();
    clear();

    // Print the constant line
    printw("Enter your word (press Esc to exit): ");

    // Print the user's input
    if (ch != 127)
    {
      printw("%s\n", input.c_str());
    }
    // Print other information based on user's choices
    std::vector<std::string> suggestions;
    if (algorithmChoice == 1)
    {
      suggestions = t.find_suggestions(input);
    }
    else if (algorithmChoice == 2)
    {
      suggestions = rbt.find_prefix(input);
    }

    if (levenshteinChoice)
    {
      std::vector<std::string> filteredSuggestions;
      printw("Levenshtein test.\n");
    }

    // Print the constant line
    printw("\nAutocomplete suggestions:\n");
    for (const auto &suggestion : suggestions)
    {
      printw("%s\n", suggestion.c_str());
      refresh();
    }
    refresh();
  }

  endwin();

  return 0;
}