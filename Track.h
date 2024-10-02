#ifndef TRACK_H
#define TRACK_H

#include <string>
#include <vector>

using namespace std;

class Track {
    protected:
    string name;
    vector<float> Audio;
    public:
    void setName(string name);
    string getName();
    virtual bool setAudio() = 0;
    static int audioCallback(const void* inputBuffer, 
                         void* outputBuffer, 
                         unsigned long framesPerBuffer, 
                         const PaStreamCallbackTimeInfo* timeInfo, 
                         PaStreamCallbackFlags statusFlags, 
                         void* userData);
    bool playAudio();
    bool exportAudio();
};

#endif