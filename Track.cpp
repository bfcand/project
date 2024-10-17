
#include "Track.h"

#include <portaudio.h>
#include <sndfile.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define SAMPLE_RATE 44100
#define MAX_SECONDS 15

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

    if(data->currentFrame >= data->numFrames){
        data->currentFrame = 0;
    }

    if(data->play == false){
      for (int j = i; j < framesPerBuffer; j++){

            out[j] = 0.0f;
            return paComplete;
        }
    }
}
return paContinue;
}

bool Track::playAudioLoop(AudioData* audioData) {

audioData->currentFrame = 0;
audioData->play = true;

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
                    audioData);

Pa_StartStream(stream);

//stopping the loop
string stopInput;
string clearBuffer;
cin>>clearBuffer;
while (trackData.play){
cin>>stopInput;
if(stopInput=="q"){
  trackData.play = false;
}
}

Pa_StopStream(stream);


Pa_Terminate();

return true;
}

AudioData* Track::gettrackData(){return &trackData;}

float Track::getAudio(int i, AudioData* data) { return data->audio[i]; }

 AudioData* Track::getdefaultAudio(){ return defaultAudio; }

int Track::getNumSamples(){return trackData.numFrames; }

void Track::setName(string name) { this->name = name; }

string Track::getName() { return name; }