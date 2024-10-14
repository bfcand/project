#ifndef SAMPLE_H
#define SAMPLE_H

#include <string>
#include <vector>

#include "Track.h"

class Sample: public Track {
protected:
int volume;
std::string fileName;
public:
Sample(std::string file):volume(100),fileName(file){}
Sample():Sample(""){}
void setfileName(std::string fileName);
std::string getfileName();
bool readAudio();
bool setAudio(); //calls the readAudio function
bool setVolume(int newVolume);
int getVolume();
};

#endif