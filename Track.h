#ifndef TRACK_H
#define TRACK_H

#include <string>
#include <vector>

using namespace std;

//abstract class
class Track {
    protected:
    string name;
    vector<float> Audio;
    public:
    void setName();
    string getName();
    virtual bool setAudio() = 0;
    bool playAudio();
    bool exportAudio();
};

#endif