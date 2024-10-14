#include "Loop.h"

#include <string>
#include <vector>

#include "Mix.h"
#include "Track.h"

// bool Loop::setAudio(){} this will become an abstract class and this version of the function
// will set audio to mix's audio track.

Loop::Loop(){}
Loop::~Loop(){}
bool Loop::setAudio(){} //sets the loop's audio as the current Mix
bool Loop::setMix(Mix* newLoop) { this->newLoop = newLoop; }