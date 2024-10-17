#ifndef MIX_H
#define MIX_H


#include<iostream>
#include<string>
#include<vector>

#include "Track.h"
#include "Sample.h"
#include "Rhythmic.h"
#include "Mix.h"

class Mix : public Track {
private:
Sample** samples;
public:
//setAudio
Mix();
~Mix();
void writeMix();
bool setAudio(); //calls the writeMix function
bool addSample(Sample* newSample);
bool deleteSample(std::string name);
Sample* findSample(std::string name);
void getSampleNames();
int getNumSamples();
};

#endif