
#include <string>
#include <vector>

#include "Track.h"

#include "Mix.h"

class Loop : public Track {
private:
Mix* newLoop;

public:
Loop();
~Loop();
bool setAudio(); //sets the loop's audio as the current Mix
bool setMix(Mix* newLoop);
};