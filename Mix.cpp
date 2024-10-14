

#include<iostream>
#include<string>
#include<vector>
#include<cmath>

#include "Track.h"
#include "Rhythmic.h"
#include "Mix.h"

using namespace std;

Mix::Mix(){
samples[6] = { nullptr };
}
Mix::~Mix(){
for (int i = 0; i < 6;i++){
delete[] samples[i];
}
}

bool Mix::createSample(){
    for (int i = 0; i < 6; i++){
        if (samples[i] == nullptr){
            samples[i] = new Sample;
        }
    }
}

bool Mix::deleteSample(string name){
    for (int i = 0; i < 6; i++){
        if(samples[i]->getName() == name){
            delete samples[i];
            return true;
        }
    }
    return false;
}

Sample* Mix::editSample(string name){
    for (int i = 0; i < 6; i++){
        if(samples[i]->getName() == name){
            return samples[i];
        }
    }
    return nullptr;     
}

void Mix::getSampleNames(){
    cout<<"Samples:"<<endl;
    for (int i = 0; i < 6;i++){
        if(samples[i]!=nullptr){
            cout<<samples[i]->getName()<<endl;
        }
    }
}

void Mix:: writeMix(){
audio.resize(22050*8);
for (int i = 0; i < 6;i++){
        if(samples[i]!=nullptr){
            for (int i = 0; i < 22050*8; i++){
                audio[i] += samples[i]->getAudio(i);
            }  
        }
    }
//rescaling if max abs amplitude exceeds 1
float maxSample = abs(*max_element(audio.begin(),audio.end()));

if(maxSample > 1){
    vector<float>::iterator itr;
    for(itr = audio.begin();itr<audio.end();itr++){
    *itr = *itr/maxSample;
    }
}
}

bool Mix::setAudio(){

trackData.audio = &audio;
trackData.numFrames = 22050*8; //2 bar pattern at 120 BPM;

return true;
}