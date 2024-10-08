

#include <iostream>
#include <vector>

#include "Rhythmic.h"

using namespace std;

#define BPM 120
#define SAMPLESPERBEAT 44100 * (60 / BPM)

bool Rhythmic::writePattern() {
  // initialising the audio vector

  vector<float> patternAudio(SAMPLESPERBEAT * 8, 0.0f); //move this into Rhythmic class?
  

  int patternIndex = 0;

  for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 16;j++){
    int samplesToWrite = (j % 2 == 0) ? 5512 : 5513;
    if (pattern[i][j] == 1) {
        //this bit isn't working, maybe use pushback instead and work out how to add zeros?
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