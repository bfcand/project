
#include "Track.h"

#include <string>

using namespace std;

void Track::setName(string name) {this->name = name;}
string Track::getName() {return name;}
// virtual bool setAudio() = 0;
bool Track::playAudio() {}
bool Track::exportAudio() {}