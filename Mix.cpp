

#include<iostream>
#include<string>
#include<vector>
#include<cmath>

#include "Track.h"
#include "Rhythmic.h"
#include "Mix.h"

using namespace std;

Mix::Mix(){
samples = new Sample*[6]{nullptr};
// for (int i = 0; i < 6;i++){
//     samples[i] = nullptr;
// };
}
Mix::~Mix(){
for (int i = 0; i < 6;i++){
    if(samples[i]!= nullptr){
delete[] samples[i];
   }
}
}

bool Mix::addSample(Sample* newSample){
    for (int i = 0; i < 6; i++){
        if (samples[i] == nullptr){
            samples[i] = newSample;
            return true;
        }
    }
    return false;
}

bool Mix::deleteSample(string name){
    for (int i = 0; i < 6; i++){
        if(samples[i]->getName() == name){
            delete samples[i];
            samples[i] = nullptr;
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
trackData.audio.resize(22050*8);
trackData.numFrames = 22050*8; //2 bar pattern at 120 BPM;
for (int i = 0; i < 6;i++){
        if(samples[i]!= nullptr){
            for (int j = 0; j < 22050*8; j++){

                 if(j >= samples[i]->getdefaultAudio()->numFrames){
                     //cout<<"sample"<< i <<" is shorter than 22050*8" << endl;
                 } else {
                    trackData.audio[j] = trackData.audio[j] + samples[i]->getAudio(j,samples[i]->getdefaultAudio());
            }  // copies across and layers the audio data from each of the samples
        }
        cout<<"wrote sample "<<i<<" to mix"<<endl;
}
}

//rescaling if max abs amplitude exceeds 1
// float maxSample = abs(*max_element(trackData.audio.begin(),(trackData.audio.end()-1)));

// if(maxSample > 1){
//     vector<float>::iterator itr;
//     for(itr = trackData.audio.begin();itr<trackData.audio.end();itr++){
//     *itr = *itr/maxSample;
//     }
// }
 }

bool Mix::setAudio(){

defaultAudio = &trackData;

return true;
}