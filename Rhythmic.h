#ifndef RHYTHMIC_H
#define RHYTHMIC_H

#include "Sample.h"

class Rhythmic : public Sample {

private:
//int pattern[2][16];
int** pattern;
std::vector<float> patternAudio;
public:
//bool setAudio(const std::string fileName);
Rhythmic();
~Rhythmic();
int** getPattern ();
void editPattern();
bool writePattern();
bool setAudio();
void printPattern(int cursorPos, int cursorRow);
void setRawMode(bool enable);
};

#endif