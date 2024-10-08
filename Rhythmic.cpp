#include <iostream>
#include <string>
#include <vector>

#include <portaudio.h>
#include <sndfile.h>

#include "Rhythmic.h"

// #include <termios.h>
// #include <unistd.h>
// #include <fcntl.h>
//option to include at a later stage

using namespace std;

#define SAMPLE_RATE 44100
#define MAX_SECONDS 15

Rhythmic::Rhythmic(){
  for (int i = 0; i < 2;i++){
    for(int j = 0; j < 16;j++){
      pattern[i][j] = 0;
    }
  }
}