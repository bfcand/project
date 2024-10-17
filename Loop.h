#ifndef LOOP_H
#define LOOP_H
#include <string>
#include <vector>

#include "Track.h"

#include "Mix.h"

class Loop : public Track {
private:
Mix* newLoop;

public:
Loop(Mix* currentMix);
Loop();
~Loop();
bool setAudio(); //sets the loop's audio as the current Mix
void setMix(Mix* newLoop);
bool writeToFile();
};

#endif