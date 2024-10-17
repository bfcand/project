#ifndef LIBRARY_H
#define LIBRARY_H
#include <string>

#include "Loop.h"

//holds all of the loops
class Library {
    private:
    Loop** loops;
    public:
    Library();
    ~Library();
    Loop* selectLoop(std::string name);
    bool Library::addLoop(Loop* loop);
    bool deleteLoop(std::string name);
    void getLoopNames();
};

#endif