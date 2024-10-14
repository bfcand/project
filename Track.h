#ifndef TRACK_H
#define TRACK_H

#include <portaudio.h>
#include <sndfile.h>

#include <string>
#include <vector>

// struct used by the Port Audio callback function.
struct AudioData {
  std::vector<float> audio;  // Pointer to the Track's audio data
  unsigned long numFrames;
  unsigned long currentFrame;  // Tracks the current playback position
};

class Track {
 protected:
  std::string name;          // the name of the track
  //std::vector<float> audio;  // where the track's audio data is stored
  AudioData trackData;  // initialisation of the struct used by the Port
  //int numFrames;

 public:
  Track(std::string name);  // constructor
  Track() : Track("") {};   // default constructor

  virtual bool setAudio() = 0;  // abstract function for writing track's audio
  
  static int audioCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData);  // callback function for Port Audio
  bool playAudioLoop();  // function for playing loop of Track's audio

  float getAudio(int i);  // getting audio sample at position i
  int getNumSamples();    // returns number of samples in audio file

  void setName(std::string name);  // setting track name
  std::string getName();           // getting the track name
};

#endif