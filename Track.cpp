
#include "Track.h"

#include <portaudio.h>
#include <sndfile.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define SAMPLE_RATE 44100
#define MAX_SECONDS 15

// struct AudioData {
//   std::vector<float>* audio;  // Pointer to the Track's audio data
//   unsigned long numFrames;
//   unsigned long currentFrame;  // Tracks the current playback position
// };

Track::Track(string name) {
  this->name = name;
  }

int Track::audioCallback(const void* inputBuffer,
                    void* outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlags,
                    void* userData){

AudioData* data = (AudioData*)userData;
float *out = (float*)outputBuffer;
(void) inputBuffer;

for (int i = 0; i < framesPerBuffer;i++){

    out[i] = data->audio[data->currentFrame];
    data->currentFrame++;

    if(data->currentFrame == data->numFrames){
        for (int j = i; j < framesPerBuffer; j++){

            out[j] = 0.0f;
            return paComplete;
        }
    }
}
return paContinue;
}

bool Track::playAudioLoop() {

trackData.currentFrame = 0;

Pa_Initialize();

PaStream *stream;

Pa_OpenDefaultStream(
                    &stream,
                    0,
                    1,
                    paFloat32,
                    44100,
                    256,
                    audioCallback,
                    &trackData);

Pa_StartStream(stream);

Pa_Sleep(2000);

Pa_StopStream(stream);

Pa_Terminate();

return true;
}

float Track::getAudio(int i) { return trackData.audio[i]; }

int Track::getNumSamples(){return trackData.numFrames; }

void Track::setName(string name) { this->name = name; }

string Track::getName() { return name; }