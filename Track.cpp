#include <string>
#include <vector>
#include <portaudio.h>
#include <sndfile.h>

#include "Track.h"

using namespace std;

// AudioData struct used by the Port Audio callback function.
struct AudioData {
  vector<float>* audioBuffer;  // Pointer to the Track's audio data
  size_t currentFrame;         // Tracks the current playback position
};


void Track::setName(string name) { this->name = name; }
string Track::getName() { return name; }


/* *** Callback function ***
as required by Port Audio library's Pa_OpenDefaultStream function */
int Track::audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags, void* userData) {

  /* Casting userData to our audio data structure (as void* must be used as per
  Port Audio callback function parameter requirements) */
  AudioData* audioStruct = static_cast<AudioData*>(userData);
  float* out = static_cast<float*>(outputBuffer);
  size_t totalFrames = audioStruct->audioBuffer->size();

}


//*** playAudio function ***
bool Track::playAudio() {
  PaStream* stream;
  PaError error;

  // Initializing PortAudio
  error = Pa_Initialize();
  if (error != paNoError) {
    return false;
    }

 // Initialisaing an AudioData struct and setting Audio to buffer.
    AudioData data;
    data.audioBuffer = &Audio;  // Setting address of Audio to audio buffer.
    data.currentFrame = 0;      // Starting from the beginning of the audio buffer

error = Pa_OpenDefaultStream(&stream,
                               0,           // 0 input channels
                               1,           // 1 output channel (mono)
                               paFloat32,   // 32-bit floating point output
                               44100,       // Sample rate
                               256,         // Frames per buffer
                               audioCallback,
                               &data);      // Pass our AudioData struct to the callback

    if (error != paNoError) {
        Pa_Terminate();
        return false;
}
}



bool Track::exportAudio() {}