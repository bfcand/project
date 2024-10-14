
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
    Loop* addLoop();
    bool deleteLoop(std::string name);
    void getLoopNames();
};