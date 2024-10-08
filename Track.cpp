#include "Track.h"

#include <fcntl.h>
#include <portaudio.h>
#include <sndfile.h>
#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define SAMPLE_RATE 44100
#define MAX_SECONDS 15

bool Track::setAudio(const string fileName) {
  trackData.ptr_audio = &audio;
  SF_INFO trackInfo;

  SNDFILE* file = sf_open(fileName.c_str(), SFM_READ, &trackInfo);
  // error handling
  if (file == nullptr) {
    cerr << "Error opening file: " << sf_strerror(file) << endl;
    return false;
  }
  cout << "File was successfully opened." << endl;

  if (trackInfo.samplerate != SAMPLE_RATE) {
    cout << "Invalid sample rate. Sample rate must be 44.1kHz. Please try "
            "another file!"
         << endl;
    return false;
  }

  if (trackInfo.channels > 2) {
    cout << "Invalid number of audio channels. Audio must be mono or stereo. "
            "Please try another file!"
         << endl;
    return false;
  }

  sf_count_t maxFrames = 0;

  if (trackInfo.channels == 2) {
    maxFrames = 15 * 2 * 44100;
  } else if (trackInfo.channels == 1) {
    maxFrames = 15 * 44100;
  } else {
    cout << "An unexpected error has occurred" << endl;
    return false;
  }
  cout << "Set max number of frames." << endl;

  // resizing the audio vector to the required length.
  audio.resize(maxFrames);

  // copying the frames into audio.
  trackData.numFrames =
      sf_readf_float(file, audio.data(), maxFrames);

  // closing file
  sf_close(file);

  if (trackInfo.channels == 1) {
    size_t originalSize = trackData.ptr_audio->size();
    trackData.ptr_audio->resize(originalSize * 2);
    for (size_t i = originalSize; i > 0; i--) {
      audio[(2 * (i - 1))] =
          audio[i - 1];  // left channel
      audio[2 * (i - 1) + 1] =
          audio[i - 1];  // right channel
    }
    trackInfo.channels = 2;    // Mark as stereo
    trackData.numFrames *= 2;  // There are twice as many frames now
  }
  return true;
}

int Track::audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags, void* audioStruct) {
  // Casting userData to our audio data structure
  AudioData* trackData = static_cast<AudioData*>(audioStruct);
  float* output = static_cast<float*>(outputBuffer);

  // Safety check for output buffer
  if (output == nullptr) {
    return paAbort;  // Abort the stream if the output buffer is invalid
  }

  // Copy audio data into the output buffer
  for (unsigned long i = 0; i < framesPerBuffer; i++) {
    // if we've reached the end of the audio sample, start again from beginning.
    if (trackData->currentFrame == trackData->numFrames) {
      trackData->currentFrame = 0;
    }

    output[2 * i] = (*trackData->ptr_audio)[trackData->currentFrame];  // Left
    output[2 * i + 1] =
        (*trackData->ptr_audio)[trackData->currentFrame + 1];  // Right
    trackData->currentFrame += 2;  // Move to the next stereo frame
  }

  return paContinue;
}

bool Track::playAudioLoop() {
  PaStream* stream;  // initialising stream
  PaError error;     // initialising error

  // Initializing PortAudio
  error = Pa_Initialize();
  if (error != paNoError) {
    return false;
  }

  // Initialising an AudioData struct and copying Audio's address into the
  // audioBuffer pointer.

  trackData.currentFrame = 0;

  error = Pa_OpenDefaultStream(
      &stream,  // location of initialised Pastream
      0,        // 0 input channels
      2,  // 2 output channel (all audio files will be converted to stereo)
      paFloat32,  // 32-bit floating point output
      44100,      // Sample rate
      256,        // Frames per buffer
      audioCallback,
      &trackData);  // Pass our AudioData struct to the callback, the userData

  if (error != paNoError) {
    Pa_Terminate();
    return false;
  }
  bool isPlaying = false;
  cout << "Enter 's' to start loop." << endl;
  string input;
  cin >> input;
  if (input == "s") {
    error = Pa_StartStream(stream);
    if (error != paNoError) {
      Pa_Terminate();
      return false;
    }
    isPlaying = true;
    cout << "Enter 'q' to stop loop." << endl;
  }

  while (isPlaying == true) {
    cin >> input;
    if (input == "q") {
      // Stop the stream
      error = Pa_StopStream(stream);
      if (error != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
      }
      isPlaying = false;
    }
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

float Track::getAudio(int i) { return audio[i]; }

void Track::setName(string name) { this->name = name; }

string Track::getName() { return name; }