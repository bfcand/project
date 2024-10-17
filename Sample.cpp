
#include<iostream>
#include<string>
#include<vector>

#include <sndfile.h>
#include <portaudio.h>


#include "Sample.h"

using namespace std;

void Sample::setfileName(string fileName){
    this->fileName = fileName;
}

string Sample::getfileName(){return fileName;};

bool Sample::readAudio() {

SF_INFO trackInfo;
trackInfo.format = 0; //as required per sndfile documentation
SNDFILE* file = sf_open(fileName.c_str(), SFM_READ, &trackInfo);

if(trackInfo.channels != 1){
    cout<<"Invalid sound file. Sound file must be mono."<<endl;
    return false;
}


trackData.audio.resize(trackInfo.frames * trackInfo.channels);

trackData.numFrames = sf_readf_float(file, trackData.audio.data(), trackInfo.frames);

sf_close(file);

return true;
}

bool Sample::setAudio(){

if(readAudio()){
    return true;
}

return false;

}

bool Sample::setVolume(int newVolume){
    if(newVolume >= 0 && newVolume <= 100){
        volume = newVolume;
        return true;
    }
    return false;
}

int Sample::getVolume(){return volume;}