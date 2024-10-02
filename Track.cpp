#include "Track.h"

#include <portaudio.h>
#include <sndfile.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define MAX_SECONDS 15

// AudioData struct used by the Port Audio callback function.
struct AudioData {
  vector<float>* audioBuffer;
  size_t numFrames;     // Pointer to the Track's audio data
  size_t currentFrame;  // Tracks the current playback position
};

void Track::setName(string name) { this->name = name; }
string Track::getName() { return name; }

/* *** Callback function ***
as required by Port Audio library's Pa_OpenDefaultStream function */
int Track::audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags, void* userData) {
    // Casting userData to our audio data structure
    AudioData* audioStruct = static_cast<AudioData*>(userData);
    float* output = static_cast<float*>(outputBuffer);

    // Safety check for output buffer
    if (output == nullptr) {
        return paAbort; // Abort the stream if the output buffer is invalid
    }

    unsigned long framesToPlay = framesPerBuffer;

    // If near the end of the buffer, adjust the frames to play
    if (audioStruct->currentFrame + framesPerBuffer > audioStruct->numFrames) {
        framesToPlay = audioStruct->numFrames - audioStruct->currentFrame;
    }

    // Copy audio data from the buffer to the output buffer
    for (unsigned long i = 0; i < framesToPlay; i++) {
        output[i] = (*audioStruct->audioBuffer)[audioStruct->currentFrame + i];
    }

    // If there are fewer frames than requested, fill remaining buffer with zeros
    for (unsigned long i = framesToPlay; i < framesPerBuffer; i++) {
        output[i] = 0.0f;
    }

    // Increment the current frame counter
    audioStruct->currentFrame += framesToPlay;

    // Return paComplete if playback is finished, otherwise paContinue
    return (audioStruct->currentFrame >= audioStruct->numFrames) ? paComplete : paContinue;
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

  error =
      Pa_OpenDefaultStream(&stream,
                           0,          // 0 input channels
                           1,          // 1 output channel (mono)
                           paFloat32,  // 32-bit floating point output
                           44100,      // Sample rate
                           256,        // Frames per buffer
                           audioCallback,
                           &data);  // Pass our AudioData struct to the callback

  error = Pa_StartStream(stream);
  if (error != paNoError) {
    Pa_Terminate();
    return false;
  }

   // Wait for the stream to finish (while stream is active)
    while (Pa_IsStreamActive(stream) == 1) {
        Pa_Sleep(100);  // Sleep to give time for the audio to process
    }

    // Stop the stream
    error = Pa_StopStream(stream);
    if (error != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
    }

    // Close the stream
    error = Pa_CloseStream(stream);
    if (error != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
    }

    // Terminate PortAudio
    Pa_Terminate();

  return true;
}

// *** setAudio for derived class Sample ***

bool Track::setAudio(const string fileName) {
  SF_INFO sfInfo;
  sfInfo.format = 0;

  SNDFILE* file = sf_open(fileName.c_str(), SFM_READ, &sfInfo);

  if (!file) {
    cerr << "Error opening file: " << sf_strerror(file) << endl;
    return false;
  }

  sf_count_t maxFrames = sfInfo.samplerate * 15;

  Audio.resize(maxFrames * sfInfo.channels);

  // Read the audio data into the vector<float> Audio
  sf_count_t numFrames = sf_readf_float(file, Audio.data(), maxFrames);

  // closing file
  sf_close(file);

  return true;
}

float Track::getAudio(int i){
  return Audio[i];
}

// bool Track::exportAudio() {}