#ifndef TRACK_H
#define TRACK_H

#include <portaudio.h>
#include <sndfile.h>

#include <string>
#include <vector>

// struct used by the Port Audio callback function.
struct AudioData {
  std::vector<float> audio;  // Stores the audio data
  unsigned long numFrames;
  unsigned long currentFrame;  // Tracks the current playback position
  bool play;
};

class Track {
 protected:
  std::string name;          // the name of the track
  AudioData trackData;  // initialisation of the struct used by the Port
  AudioData* defaultAudio; // pointer to selected audio struct

 public:
  Track(std::string name);
  Track();   // default constructor
  virtual ~Track();
  virtual bool setAudio() = 0;  // abstract function for writing track's audio
  
  static int audioCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData);  // callback function for Port Audio
  bool playAudioLoop(AudioData* audioData);  // function for playing loop of Track's audio

  AudioData* gettrackData();
  AudioData* getdefaultAudio();
  float getAudio(int i, AudioData* data);  // getting audio sample at position i
  int getNumSamples();    // returns number of samples in audio file

  void setName(std::string name);  // setting track name
  std::string getName();           // getting the track name
};

#endif