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
  start_color();        // Enable color support
  init_pair(1, COLOR_GREEN, COLOR_BLACK);  // Define color pair 1 (Green text on black background)
  init_pair(2, COLOR_BLUE, COLOR_BLACK);   // Define color pair 2 (Blue text on black background)

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
  printw("Choose an autocomplete algorithm: \n\t1 for MAST\n\t2 for Red Black Tree\n\t3 for Levenshtein\n");
  refresh();
  algorithmChoice = getch() - '0';
  printw("%d\n", algorithmChoice);
  refresh();

  while (algorithmChoice != 1 && algorithmChoice != 2 && algorithmChoice != 3)
  {
    attron(COLOR_PAIR(2)); // Blue text
    printw("Invalid algorithm choice. Please choose 1, 2 or 3.\n");
    attroff(COLOR_PAIR(2));
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
  case 3:
    chosenAlgorithm = "Levenshtein";
    break;
  default:
    break;
  }
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  storingTime = duration.count();
  attron(COLOR_PAIR(1)); // Green text
  printw("\tTime taken by function: %ld milliseconds.\n", storingTime);
  attroff(COLOR_PAIR(1));

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

  attron(COLOR_PAIR(1)); // Green text
  printw("\tMemory used by one node: %s bytes\n", memoryUsedNode.c_str());
  printw("\tMemory used by all nodes: %s bytes\n", memoryUsedData.c_str());
  attroff(COLOR_PAIR(1));

  // Get user's decision whether to use Levenshtein or not
  if (algorithmChoice == 1) {
    printw("\nDo you want to use the Levenshtein automata? \n\t1 for Yes\n\t0 for No\n");
    refresh();
    levenshteinChoice = getch() - '0';
    printw("%d\n", levenshteinChoice);
    refresh();
  }
  else if (algorithmChoice == 2) {
    levenshteinChoice = 0;
  }
  else {
    levenshteinChoice = 1;
  }

  while (levenshteinChoice != 1 && levenshteinChoice != 0)
  {
    attron(COLOR_PAIR(2)); // Blue text
    printw("Invalid choice. Please type 1 or 0.\n");
    attroff(COLOR_PAIR(2));
    refresh();
    levenshteinChoice = getch() - '0';
    printw("%d\n", levenshteinChoice);
    refresh();
  }

  levenshteinEnabled = (levenshteinChoice == 1) ? "TRUE" : "FALSE";

  if (levenshteinChoice)
  {
    printw("\nChoose Levenshtein distance (0, 1, or 2): ");
    refresh();
    levenshteinDistance = getch() - '0';
    printw("%d\n", levenshteinDistance);
    refresh();

    while (levenshteinDistance != 0 && levenshteinDistance != 1 && levenshteinDistance != 2)
    {
      attron(COLOR_PAIR(2)); // Blue text
      printw("Unavailable Levenshtein distance, choose 0, 1, or 2: ");
      attroff(COLOR_PAIR(2));
      refresh();
      levenshteinDistance = getch() - '0';
      printw("%d\n", levenshteinDistance);
      refresh();
    }
  }

  clear();
  refresh();
  mvprintw(0, 0, "Enter your word (press ");
  attron(A_BOLD);
  mvprintw(0, 23, "Esc");
  attroff(A_BOLD);
  mvprintw(0, 26, " to exit): ");
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
    attron(COLOR_PAIR(2)); // Green text
    getmaxyx(stdscr, maxY, maxX);
    attron(A_BOLD);
    mvprintw(maxY - 7, maxX - 30, "Information:");
    attroff(A_BOLD);
    mvprintw(maxY - 6, maxX - 30, "Choosed Algorithm: %s", chosenAlgorithm.c_str());
    mvprintw(maxY - 5, maxX - 30, "Levenshtein Enabled: %s", levenshteinEnabled.c_str());
    mvprintw(maxY - 4, maxX - 30, "Storing time: %ld ms", storingTime);
    mvprintw(maxY - 3, maxX - 30, "Memory Used(Node): %s", memoryUsedNode.c_str());
    mvprintw(maxY - 2, maxX - 30, "Memory Used(Data): %s", memoryUsedData.c_str());
    mvprintw(maxY - 1, maxX - 30, "Number of words: %d", numberOfWords);
    attroff(COLOR_PAIR(2));
    refresh();

    // Print the constant line
    mvprintw(0, 0, "Enter your word (press ");
    attron(A_BOLD);
    mvprintw(0, 23, "Esc");
    attroff(A_BOLD);
    mvprintw(0, 26, " to exit): ");
    refresh();

    printw("%s", input.c_str());
    refresh();
    cursorToInput.first = 37 + input.size();
    cursorToInput.second = 0;

    // Print other information based on user's choices
    std::vector<std::string> suggestions;
    std::vector<std::string> levSuggestions;
    start = std::chrono::high_resolution_clock::now();
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
    else if (algorithmChoice == 3) {
      std::ifstream inputFile(currentDirectory + "/assets/data/american-english-sorted.txt");
      LevenshteinDFA lev(input, levenshteinDistance);
      lev.generateDFA();
      std::string currentWord;
      lev.estimateMemoryUsage(memory);
      memoryUsedNode = memory.first;
      memoryUsedData = memory.second;
      while (std::getline(inputFile, currentWord)) {
        suggestions.push_back(currentWord);
      }
      levSuggestions = lev.find_suggestions(suggestions);
    }
    stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long int, std::ratio<1, 1000000>> dur = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    if (algorithmChoice == 3) storingTime = dur.count() / 1000;

    attron(COLOR_PAIR(2)); // Blue text
    mvprintw(maxY - 4, maxX - 30, "Storing time: %ld ms", storingTime);
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(1)); // Green text
    mvprintw(1, 0, "\nTime taken by suggestions: %ld microseconds.", dur.count());
    attroff(COLOR_PAIR(1));
    // Print the constant line
    attron(A_BOLD);
    printw("\nAutocomplete suggestions:\n");
    attroff(A_BOLD);
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
