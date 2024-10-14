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
bool createSample();
bool deleteSample(std::string name);
Sample* editSample(std::string name);
void getSampleNames();
};

#endif