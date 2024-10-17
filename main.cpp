#include <portaudio.h>
#include <sndfile.h>
#include <termios.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Library.h"
#include "Loop.h"
#include "Mix.h"
#include "Rhythmic.h"
#include "Sample.h"
#include "Track.h"

using namespace std;

int main(void) {
  cout << "Music Sampler" << endl;

  bool continueProgram = true;        // setting initial program state
  bool continueMainSelection = true;  // setting initial program state
  bool continueSubSelection = true;
  char selection;

  Mix mix;
  Library library;

  while (continueProgram) {
    selection = 0;
    cout << "Main menu:\nType and enter: 's' to edit, delete or create a "
            "sample, 'm' "
            "to hear mix, or 'l' "
            "to create, listen to or write a loop.\nType and enter 'q' to "
            "quit."
         << endl;
    for (;;) {
      if (!(cin >> selection)) {
        cout << "Invalid input, please try again." << endl;
        cin >> selection;
      } else if (selection != 's' && selection != 'l' && selection != 'q') {
        cout << "Invalid input, please try again." << endl;
      } else {
        break;
      }
    }

    // Sample
    if (selection == 's') {
      continueMainSelection = true;

      while (continueMainSelection == true) {
        continueSubSelection = true;
        selection = 0;
        cout << "\nSample menu.\nType and enter: 'a' to create new sample, 'e' "
                "to "
                "edit "
                "existing sample or 'd' to delete existing sample.\nType and "
                "enter 'b' to return to the main menu."
             << endl;
        for (;;) {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cin >> selection;
          if (selection != 'a' && selection != 'e' && selection != 'd' &&
              selection != 'b') {
            cout << "Invalid selection, please try again." << endl;
          } else {
            break;
          }
        }

        if (selection == 'a') {  // 'add sample' option
          //continueSubSelection = true;

          Rhythmic* newSample = new Rhythmic;
          string newFileName = "";

          while (continueSubSelection == true) {
            for (;;) {
              cout << "Type and enter 'y' to proceed with adding file, or 'b' "
                      "to go back to the sample menu"
                   << endl;
              cin.clear();
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cin >> selection;

              if (selection == 'b') {
                continueSubSelection = false;
                delete newSample;
                continueSubSelection = false;
                break;

              } else if (selection == 'y') {
                cout
                    << "Please enter the name of the audio file you would like "
                       "to "
                       "read. Please include file extension such as '.wav'"
                    << endl;
                cin >> newFileName;
                newSample->setfileName(newFileName);

                if (newSample->readAudio()) {
                  cout << "Succesfully read file." << endl;
                  break;
                } else {
                  cout << "Error with reading the file, please check the file "
                          "name "
                          "and try again."
                       << endl;
                }

              } else {
                cout << "Invalid selection, please try again." << endl;
              }
            }


            if (continueSubSelection) { //if we have read an audio file
              // setting name of sample
              string* tempName = new string;
              cout
                  << "Please choose a name for your sample of max 12 characters"
                  << endl;
              for (;;) {
                cin >> *tempName;
                if (tempName->size() > 12) {
                  cin.clear();
                  cin.ignore(numeric_limits<streamsize>::max(), '\n');
                  cout << "Name exceeds 12 characters in length. Please enter "
                          "another name."
                       << endl;
                } else if (*tempName == "") {
                  cin.clear();
                  cin.ignore(numeric_limits<streamsize>::max(), '\n');
                  cout << "Please enter at least one character." << endl;
                } else {
                  newSample->setName(*tempName);
                  delete tempName;
                  break;
                }
              }

              // adding the new sample to mix
              if (mix.addSample(newSample)) {
                cout << newSample->getName()
                     << " was successfuly added to the mix!" << endl;
              }
              cin.clear();
              cin.ignore(numeric_limits<streamsize>::max(), '\n');

              newSample->editSample();   // initial pattern edit
              newSample->writeSample();  // initial write of pattern audio
              newSample->setAudio();  // setting pattern audio as default audio
                                      // source
              for (;;) {
                selection = 0;

                cout << "Type and enter: 'p' to play and stop audio pattern, "
                        "'e' to continue editing pattern, or 'b' to return to "
                        "sample menu."
                     << endl;
                cin >> selection;
                if (selection == 'e') {
                  getchar();  // gets \n character
                  newSample->editSample();
                  newSample->writeSample();
                } else if (selection == 'p') {
                  newSample->playAudioLoop(newSample->getdefaultAudio());
                } else if (selection == 'b') {
                  continueSubSelection = false;
                  continueMainSelection = false;
                  break;
                } else {
                  cout << "Invalid selection, please try again." << endl;
                }
              }
            }
          }
        } else if (selection == 'e') {  // edit existing sample
        string* nameInput = new string;
        while (continueSubSelection = true) {
        mix.getSampleNames();
          cout << "Please enter the name of the sample that you would like to "
                  "edit:"
               << endl;
          cin >> *nameInput;
          Sample* sampleToEdit = mix.findSample(*nameInput);

          if (sampleToEdit == nullptr) {
            cout << "Sample: " << *nameInput << " does not exist." << endl;

            for (;;) {
              cout << "Type and enter 'y' to try again, or b to return to the sample menu."
                   << endl;
              cin.clear();
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cin >> selection;

              if (selection == 'b') {
                delete nameInput;
                continueSubSelection = false;
                continueSubSelection = false;
                break;

              } else if (selection == 'y') {
                break;
              } else {
                cout<<"Invalid selection, please try again."<<endl;}
            } 
          } else {
            sampleToEdit->editSample();
            // sampleToEdit->writePattern();
          } 
          }
          
        } else if (selection == 'd') {  // delete existing sample

      } else if (selection == 'b') {  // go back to main menu
        continueSubSelection = false;
        continueMainSelection = false;
      } else {
        cout<<"Invalid selection, please try again."<<endl;
      }

      }
      
    }

    // Mix
    if (selection == 'm') {
      selection = 0;
      cout << "Type and enter 'p' to play/stop loop, or 'b' to return to "
              "main menu."
           << endl;
      for (;;) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> selection;
        if (selection == 'p') {
          if (mix.getNumSamples() < 1) {
            cout << "Nothing to play, no samples in mix! Return to main menu "
                    "to add a new sample."
                 << endl;
          } else {
            mix.playAudioLoop(mix.gettrackData());
          }
        } else if (selection == 'b') {
          break;
        } else {
          cout << "Invalid input, please try again." << endl;
        }
      }
    }

    // Loop
    if (selection == 'l') {
      selection = 0;
      cout << "Type and enter: 'a' to add loop, 'd' to delete loop, 'v' to "
              "view and play loops, 'w' to write loop to hard drive, or 'b' to "
              "go back."
           << endl;
      for (;;) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> selection;
        if (selection != 'a' && selection != 'd' && selection != 'w') {
        }
      }
    }

    // Quit
    if (selection == 'q') {  // quit
      while (true) {
        cout << "Are you sure you want to quit? All loops unwritten to file "
                "shall be lost.\nType and enter 'y' to confirm or 'n' to go "
                "back."
             << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> selection;
        if (selection == 'y') {
          cout << "Okay, goodbye!" << endl;
          continueProgram = false;
          break;
        } else if (selection == 'n') {
          break;
        } else {
          cout << "Invalid selection, please try again." << endl;
        }
        // exit this while loop to return to programContinue while loop.
      }
    }
  }
  return 0;
}