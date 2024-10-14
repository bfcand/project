
#include "Library.h"

#include <iostream>
#include <string>

#include "Loop.h"

using namespace std;

Library::Library(){
loops[10] = { nullptr };
};

Library::~Library(){
    for (int i = 0; i < 10; i++){
        delete loops[i];
    }
};
Loop* Library::selectLoop(string name) {
    for (int i = 0; i < 10; i++){
        if(loops[i]!=nullptr){
            if(loops[i]->getName() == name){
                return loops[i];
            }
        }
    }
    return nullptr;
}
Loop* Library::addLoop() {
    for (int i = 0; i < 10; i++){
        if(loops[i]==nullptr){
            loops[i] == new Loop;
            return loops[i];
        }
    }
    return nullptr;
}
bool Library::deleteLoop(string name) {
    for (int i = 0; i < 10; i++){
        if(loops[i]!=nullptr){
            if(loops[i]->getName() == name){
                delete loops[i];
                return true;
            }
        }
    }
    return false;
}
void Library::getLoopNames() {
    cout<<"Loops:"<<endl;
    for (int i = 0; i < 10; i++){
        if(loops[i]!=nullptr){
            cout<<loops[i]->getName()<<endl;
        }
    }
}