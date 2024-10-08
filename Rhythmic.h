#ifndef RHYTHMIC_H
#define RHYTHMIC_H

#include "Track.h"

class Rhythmic : public Track {

private:
int pattern[2][16];

public:
//bool setAudio(const string fileName);
Rhythmic();
int* getPattern ();
void editPattern();
bool writePattern();
void printPattern(int cursorPos, int cursorRow);
void setRawMode(bool enable);
};

#endif