
#include "Library.h"

#include <iostream>
#include <string>

#include "Loop.h"

using namespace std;

Library::Library(){
loops = new Loop*[10]{nullptr};
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
bool Library::addLoop(Loop* loop) {
    for (int i = 0; i < 10; i++){
        if(loops[i]==nullptr){
            loops[i] = loop;
            return true;
        }
    }
    return false;
}
bool Library::deleteLoop(string name) {
    char selection = 0;
    for (int i = 0; i < 10; i++){
        if(loops[i]!=nullptr){
            if(loops[i]->getName() == name){
                cout << "Are you sure you want to delete "<<name<<"?\nType and enter 'y' for yes or 'b' to go back"<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin>>selection;
            if (selection == 'y') {
                delete loops[i];
                return true;
            } else if (selection == 'b') {
                return false;
              } else {
                cout<<"Invalid selection, please try again."<<endl;
                }
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