
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>

#include "Rhythmic.h"

// function for changing terminal settings
void Rhythmic::setRawMode(bool enable) {
  struct termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  if (enable) {
    tty.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
  } else {
    tty.c_lflag |= (ICANON | ECHO);  // Re-enable canonical mode and echo
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}