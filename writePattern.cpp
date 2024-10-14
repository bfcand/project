

#include <iostream>
#include <vector>

#include "Rhythmic.h"

using namespace std;

#define BPM 120
#define SAMPLESPERBEAT 44100 * (60 / BPM)

bool Rhythmic::writePattern() {
  // initialising the audio vector

  patternAudio.resize(22050 * 8, 0.0f); //move this into Rhythmic class?
  

  int patternIndex = 0;

  for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 16;j++){
    int samplesToWrite = (j % 2 == 0) ? 5512 : 5513; //first or second semi quaver
    if (pattern[i][j] == 1) {
      int audioIndex = 0;

      for (int k = patternIndex; k < patternIndex + samplesToWrite; k++) {
        patternAudio[k] = audio[audioIndex];
        audioIndex++;
      }
    }
  }
  }
  audio = patternAudio;
  return true;
}