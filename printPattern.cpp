

#include<iostream>
#include <cstdio>
#include <termios.h>
#include <unistd.h>

#include "Rhythmic.h"

void setRawMode(bool enable) {
  struct termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  if (enable) {
    tty.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
  } else {
    tty.c_lflag |= (ICANON | ECHO);  // Re-enable canonical mode and echo
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}


void Rhythmic::printPattern(int cursorPos, int cursorRow){

for (int i = 0; i < 2; i++) {

    if (cursorPos == 0 && i == cursorRow) {
      std::cout << "[";
    } else {
      std::cout << "[ ";
    }

    for (int j = 0; j < 16; j++) {

      if (j % 4 == 0 && j > 0) {
        if (j == cursorPos && i == cursorRow) {
          std::cout << " |";
        } else {
          std::cout << "| ";
        }
      }
      // Highlight the current cursor position
      if (j == cursorPos && i == cursorRow) {
        std::cout << "(" << pattern[i][j] << ")";
      } else if (j == (cursorPos - 1) && i == cursorRow) {
        std::cout << pattern[i][j];
      } else {
        std::cout << pattern[i][j] << " ";
      }

    }
    std::cout << "]" << std::endl;

  }

}