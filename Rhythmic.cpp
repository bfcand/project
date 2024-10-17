#include "Rhythmic.h"

#include <portaudio.h>
#include <sndfile.h>

#include <termios.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define SAMPLE_RATE 44100
#define MAX_SECONDS 15

Rhythmic::Rhythmic() {
  pattern = new int*[2];
  for (int i = 0; i < 2; i++) {
    pattern[i] = new int[16]{0};
  }
}

Rhythmic::~Rhythmic() {
  for (int i = 0; i < 2; i++) {
    delete pattern[i];
  }
}

int** Rhythmic::getPattern() { return pattern; }

void Rhythmic::editSample() { // editSample for the Rhythmic derived class is a rhythm pattern editor
  int cursorPos = 0;  // Current column
  int cursorRow = 0;  // Current row (0 or 1)

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
    } else if (ch == '\n') {  // Quit on return/enter key
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

bool Rhythmic::writeSample() { // writeSample = writePattern for Rhythmic class
  
  patternData.audio.resize(22050 * 8, 0.0f);  // resizing the audio vector to fit the desired number of samples
  patternData.numFrames = 22050*8; //setting num frames to patternData

  vector<float>::iterator itr_pattern = patternData.audio.begin();

  int patternWriteIndex = 0;
  int semiQuaverLength;

  // for bars 1 and 2
  for (int i = 0; i < 2; i++) {

    // for semi quavers 1 to 16
    for (int j = 0; j < 16; j++) {
      semiQuaverLength =(j % 2 == 0) ? 5512 : 5513;  // first or second semi quaver

      if (pattern[i][j] == 1) {
        for (int i = 0; i < trackData.numFrames;i++){

          if (itr_pattern >= (patternData.audio.end()-1)) {
            break;
          }
          *itr_pattern = trackData.audio[i];  // copying audio sample into pattern
          itr_pattern++;
        }
      }
      patternWriteIndex += semiQuaverLength;
      itr_pattern = patternData.audio.begin() + patternWriteIndex;  // positining itr to next semiquaver.
    }
  }
  
  
return true;
}

bool Rhythmic::setAudio(){

defaultAudio = &patternData;

return true;
}

//function for printing pattern in the terminal
void Rhythmic::printPattern(int cursorPos, int cursorRow) {
  cout<<"Please write a rhythmic pattern for your sample. Once finished, press 'enter' to continue."<<endl;
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

AudioData* Rhythmic::getPatternData(){return &patternData;}

float Rhythmic::getPatternAudio(int i){return patternData.audio[i];};

//function for modifying pattern in the terminal
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