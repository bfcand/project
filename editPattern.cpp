
#include <termios.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

#include "Rhythmic.h"


void Rhythmic::editPattern() {

  int cursorPos = 0;           // Current column
  int cursorRow = 0;           // Current row (0 or 1)

  setRawMode(true);  // Enter raw mode
  char ch;

  printPattern(cursorPos, cursorRow);

  while (true) {
    ch = getchar();
    if (ch == '\033') {  // Escape sequence for arrow keys
      getchar();         // Skip the '['
      ch = getchar();
      if (ch == 'A') {                    // Up arrow
        cursorRow = (cursorRow + 1) % 2;  // Switch between row 0 and 1
      } else if (ch == 'B') {             // Down arrow
        cursorRow = (cursorRow + 1) % 2;  // Switch between row 0 and 1
      } else if (ch == 'C') {             // Right arrow
        cursorPos = (cursorPos + 1) % 16;
      } else if (ch == 'D') {               // Left arrow
        cursorPos = (cursorPos + 15) % 16;  // Move left, wrap around
      }
    } else if (ch == 'q') {  // Quit on 'q' key
      break;
    } else if (ch == '1') {  // User can manually toggle between 0 and 1
      pattern[cursorRow][cursorPos] = 1;
    } else if (ch == '0') {  // User can manually toggle to 0
      pattern[cursorRow][cursorPos] = 0;
    }

    std::cout << "\033[H\033[J";  // Clear screen
    printPattern(cursorPos, cursorRow);
  }

  setRawMode(false);  // Exit raw mode
}