#include "Loop.h"

#include <string>
#include <vector>

#include "Mix.h"
#include "Track.h"

using namespace std;

// bool Loop::setAudio(){} this will become an abstract class and this version of the function
// will set audio to mix's audio track.

Loop::Loop(){}

Loop::~Loop(){}

bool Loop::setAudio(){
trackData.audio = newLoop->getdefaultAudio()->audio; // copying the audio from the mix into the the loop's trackData audio vector
trackData.numFrames = newLoop->getdefaultAudio()->numFrames; // copying the number of frames from the mix into the loop's trackData frames variable
defaultAudio = &trackData;
return true;
} //sets the loop's audio to audio in Mix

void Loop::setMix(Mix* newLoop) { this->newLoop = newLoop; } //sets the pointer to the mix for copying data across

bool Loop::writeToFile(){
    vector<float> loopAudioToWrite;
    string fileName;
    int numLoops = 0;

    cout<<"Please enter a name for the loop: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin>>fileName;
    fileName += ".wav";
    cout<<"How many iterations of the loop would like to write to the file? (between 1 and 8)"<<endl;
    for(;;){
    if(cin>>numLoops){
        if(numLoops > 0 && numLoops < 9){
            break;
        }
    } else {
        cout<< "Please enter a valid number between 1 and 8."<<endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    }

    int writeIndex = 0;
    loopAudioToWrite.resize(numLoops*(trackData.audio.size()));
    cout<<"size of loopAudioToWrite: "<<loopAudioToWrite.size()<<endl;
    cout<<"resized write vector to: "<<loopAudioToWrite.size()<<endl;
    
    for (int i = 0; i < numLoops; i++){

        cout<<"i = "<<i<<" writeIndex = "<<writeIndex<<endl;
        cout<<"trackData audio size: "<<trackData.audio.size()<<endl;

        for (int j = 0; j < trackData.audio.size();j++){
            if (j == 0){
                cout<<"j is 0"<<endl;
            }
            if(writeIndex >= loopAudioToWrite.size()){
                break;
            }
            loopAudioToWrite[writeIndex] = trackData.audio[j];
            // if(i == 0){
            //     cout<<"wrote sample"<<endl;
            // }
            writeIndex++;
        }

    }
cout<<"line 74"<<endl;
    SF_INFO trackInfo;
    trackInfo.samplerate = 44100; //something not right with this part of the code
    trackInfo.channels = 1;
    trackInfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    const float* ptr_loopAudioToWrite = &loopAudioToWrite[0];

    SNDFILE* file = sf_open(fileName.c_str(), SFM_WRITE, &trackInfo);
    if(!file){
        cerr  << "Error opening files: "<<sf_strerror(nullptr) << endl;
        return 1;
    }
    sf_count_t numFrames = loopAudioToWrite.size();
    cout<<numFrames<<endl;
    sf_writef_float(file, ptr_loopAudioToWrite,numFrames);
cout<<"line 90"<<endl;
    sf_close(file);
    cout<<"line 92"<<endl;
    return true;
}