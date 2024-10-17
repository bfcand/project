#ifndef RHYTHMIC_H
#define RHYTHMIC_H

#include "Sample.h"

class Rhythmic : public Sample {

private:
int** pattern;
AudioData patternData; //a second audio data struct to hold the pattern audio information.
public:
//bool setAudio(const std::string fileName);
Rhythmic();
~Rhythmic();
int** getPattern ();
void editSample();
bool writeSample();
bool setAudio();
void printPattern(int cursorPos, int cursorRow);
void setRawMode(bool enable);
AudioData* getPatternData();
float getPatternAudio(int i);
};

#endif