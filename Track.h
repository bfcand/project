#ifndef TRACK_H
#define TRACK_H

#include <string>
#include <vector>
#include <portaudio.h>
#include <sndfile.h>

using namespace std;

// struct required by the Port Audio callback function.
struct AudioData {
  vector<float>* audio;  // Pointer to the Track's audio data
  size_t numFrames;
  size_t currentFrame;  // Tracks the current playback position
};

class Track {
    protected:
    string name;
    vector<float> audioData;
    AudioData trackData;
    public:
    static int audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags, void* userData);
    bool playAudioLoop();
    virtual bool setAudio(const string fileName); // = 0;

    void setName(string name);
    string getName();
    float getAudio(int i);
    int getNumFrames(){return trackData.numFrames;}
};


#endif