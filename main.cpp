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
    cout << "Main menu:\nType and enter: 's' to create, edit, or delete a "
            "sample, 'm' "
            "to hear mix, or 'l' "
            "to create, listen to or write a loop.\nType and enter 'q' to "
            "quit."
         << endl;
    for (;;) {
      if (!(cin >> selection)) {
        cout << "Invalid input, please try again." << endl;
        cin >> selection;
      } else if (selection != 's' && selection != 'l' && selection != 'm' &&
                 selection != 'q') {
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
        cout << "Sample menu.\nType and enter: 'a' to create new sample, 'e' "
                "to "
                "edit "
                "existing sample or 'd' to delete existing sample.\nType and "
                "enter 'b' to return to the main menu."
             << endl;
        for (;;) {
          //cin.clear();
          //cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cin >> selection;
          if (selection != 'a' && selection != 'e' && selection != 'd' &&
              selection != 'b') {
            cout << "Invalid selection, please try again." << endl;
          } else {
            break;
          }
        }

        if (selection == 'a') {  // 'add sample' option
          // continueSubSelection = true;

          Rhythmic* newSample = new Rhythmic;
          string newFileName = "";

          while (continueSubSelection == true) {
            for (;;) {
              cout << "Type and enter 'y' to proceed with adding file, or 'b' "
                      "to go back to the sample menu"
                   << endl;
              cin >> selection;

              if (selection == 'b') {
                delete newSample;
                continueSubSelection = false;
                break;

              } else if (selection == 'y') {
                if (mix.getNumSamples() >= 6) {
                  cout << "Max number of samples reached. Please delete a sample "
                          "first before adding a new one."
                       << endl;
                  continueSubSelection = false;
                  break;
                }

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
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
              }
            }

            if (continueSubSelection) {  // if we have read an audio file
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
              } else {
                cout << "There was an error adding the sample. Please try again."<<endl;
                break;
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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (selection == 'e') {
                  newSample->editSample();
                  newSample->writeSample();
                } else if (selection == 'p') {
                  newSample->playAudioLoop(newSample->getdefaultAudio());
                } else if (selection == 'b') {
                  continueSubSelection = false;
                  break;
                } else {
                  cout << "Invalid selection, please try again." << endl;
                }
              }
            }
          }
        } else if (selection == 'e') {  // edit existing sample
          string* nameInput = new string;
          Sample* sampleToEdit = nullptr;
          while (continueSubSelection == true) {
            for (;;) {
              cout << "Type and enter 'y' to proceed to edit file, or 'b' to "
                      "return to "
                      "the sample menu."
                   << endl;
              cin >> selection;

              if (selection == 'b') {
                delete nameInput;
                delete sampleToEdit;
                continueSubSelection = false;
                break;

              } else if (selection == 'y') {
                break;
              } else {
                cout << "Invalid selection, please try again." << endl;
              }
            }
            while (continueSubSelection) {
              mix.getSampleNames();
              cout << "Please enter the name of the sample that you would like "
                      "to "
                      "edit:"
                   << endl;
              cin >> *nameInput;
              sampleToEdit = mix.findSample(*nameInput);
              getchar();  // getting '\n' character from terminal
              if (sampleToEdit == nullptr) {
                cout << "Sample '" << *nameInput << "' does not exist." << endl;
                break;
              } else {
                delete nameInput;  // deallocating dynamic temporary string
                sampleToEdit->editSample();  // entering pattern editor mode
                sampleToEdit
                    ->writeSample();  // writing new pattern to audio vector
                for (;;) {
                  selection = 0;

                  cout
                      << "Type and enter: 'p' to play and stop audio pattern, "
                         "'e' to continue editing pattern, or 'b' to return to "
                         "sample menu."
                      << endl;
                  cin >> selection;
                  if (selection == 'e') {
                    getchar();  // gets \n character
                    sampleToEdit->editSample();
                    sampleToEdit->writeSample();
                  } else if (selection == 'p') {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> selection;
                    sampleToEdit->playAudioLoop(
                        sampleToEdit->getdefaultAudio());
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
          }

        } else if (selection == 'd') {  // delete existing sample
          string* nameInput = new string;
          while (continueSubSelection == true) {
            for (;;) {
              cin.clear();
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cout << "Type and enter 'y' to delete a sample, or 'b' to return "
                      "to the sample menu."
                   << endl;
              cin >> selection;
              if (selection == 'b') {
                delete nameInput;
                continueSubSelection = false;
                break;
              } else if (selection == 'y') {
                mix.getSampleNames();
                cout << "Please enter the name of the sample that you would "
                        "like "
                        "to "
                        "delete:"
                     << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> *nameInput;

                if (mix.deleteSample(*nameInput)) {
                  cout << "Sample successfully deleted" << endl;
                  delete nameInput;
                  continueSubSelection = false;
                  break;
                } else {
                  cout << "Sample: " << *nameInput << " was not deleted"
                       << endl;
                }

              } else {
                cout << "Invalid selection, please try again." << endl;
              }
            }
          }

        } else if (selection == 'b') {  // go back to main menu
          continueSubSelection = false;
          continueMainSelection = false;
        } else {
          cout << "Invalid selection, please try again." << endl;
        }
      }
    }

    else if (selection == 'm') {  // Mix
      selection = 0;
      if (!mix.writeMix()) {
        cout << "Nothing to play, no samples in mix! Return to main menu to "
                "add a new sample."
             << endl;
      } else {
        for (;;) {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cout << "Type and enter 'p' to play/stop loop, or 'b' to return to "
                  "main menu."
               << endl;
          cin >> selection;
          if (selection == 'p') {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            mix.playAudioLoop(mix.gettrackData());
          } else if (selection == 'b') {
            break;
          } else {
            cout << "Invalid input, please try again." << endl;
          }
        }
      }
    }

    // Loop
    else if (selection == 'l') {
      continueMainSelection = true;
      while (continueMainSelection) {
        selection = 0;
        cout << "Type and enter: 'c' to create loop from mix, 'd' to delete a "
                "loop, 'v' to "
                "view and play loops, 'w' to write a loop to the hard drive, "
                "or 'b' "
                "to "
                "go back."
             << endl;
        for (;;) {
          //cin.clear();
          //cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cin >> selection;

          if (selection != 'c' && selection != 'd' && selection != 'v' &&
              selection != 'w' && selection != 'b') {
            cout << "Invalid selection. Please try again." << endl;
          } else {
            break;
          }
        }

        if (selection == 'c') {
          continueSubSelection = true;
          while (continueSubSelection == true) {
            cout << "Type and enter 'y' to create a loop out of the current "
                    "mix, "
                    "or 'b' to go back."
                 << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> selection;

            if (selection == 'y') {
              if (!mix.writeMix()) {
                cout << "Nothing to create, no samples in mix! Return to main "
                        "menu "
                        "to "
                        "add a new sample."
                     << endl;
                continueSubSelection = false;
              } else if (library.getNumLoops() >= 10) {
                cout
                    << "Reached maximum of 10 loops in library. Please return "
                       "to loop menu and delete a loop before adding a new one."
                    << endl;
                continueSubSelection = false;
              } else {
                Loop* newLoop = new Loop(&mix);
                newLoop->setAudio();

                string* tempName = new string;  // choosing name for loop
                cout << "Please choose a name for your loop of max 12 "
                        "characters"
                     << endl;
                for (;;) {
                  cin >> *tempName;
                  if (tempName->size() > 12) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Name exceeds 12 characters in length. Please "
                            "enter "
                            "another name."
                         << endl;
                  } else if (*tempName == "") {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Please enter at least one character." << endl;
                  } else {
                    newLoop->setName(*tempName);
                    delete tempName;
                    if (library.addLoop(newLoop)) {
                      cout << "New loop successfully created. Returning to "
                              "loop menu."
                           << endl;
                      continueSubSelection = false;
                      break;
                    } else {
                      cout << "Something went wrong. Please try adding loop "
                              "again."
                           << endl;
                      continueSubSelection = false;
                      break;
                    }
                  }
                }
              }
            } else if (selection == 'b') {
              continueSubSelection = false;
            }
          }

        } else if (selection == 'd') {
          continueSubSelection = true;
          string* nameInput = new string;
          while (continueSubSelection == true) {
            for (;;) {
              cin.clear();
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cout << "Type and enter 'y' to delete a loop, or 'b' to return "
                      "to the loop menu."
                   << endl;
              cin >> selection;
              if (selection == 'b') {
                delete nameInput;
                continueSubSelection = false;
                break;
              } else if (selection == 'y') {
                library.getLoopNames();
                cout << "Please enter the name of the loop that you would "
                        "like "
                        "to "
                        "delete:"
                     << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> *nameInput;

                if (library.deleteLoop(*nameInput)) {
                  cout << "Loop '" << *nameInput << "' successfully deleted"
                       << endl;
                  delete nameInput;
                } else {
                  cout << "Loop: " << *nameInput << " was not deleted" << endl;
                }
              } else {
                cout << "Invalid selection, please try again." << endl;
              }
            }
          }
        } else if (selection == 'v') {  // view and play loops
          continueSubSelection = true;
          while (continueSubSelection) {
            library.getLoopNames();
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Type and enter 'y' to view and play loops, or 'b' to "
                    "return "
                    "to the loop menu."
                 << endl;
            cin >> selection;
            if (selection == 'b') {
              continueSubSelection = false;
            } else if (selection == 'y') {
              string* tempName = new string;
              Loop* selectedLoop = nullptr;
              cout << "Please enter the name of the loop that you would like "
                      "to play: "
                   << endl;
              cin.clear();
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cin >> *tempName;
              selectedLoop = library.selectLoop(*tempName);
              if (selectedLoop ==
                  nullptr) {  // get pointer to the loop with inputted name
                cout << "Loop '" << *tempName
                     << "' does not exist. Please try again." << endl;
                delete tempName;
              } else {
                cout << "Type and enter 'p' to play/stop loop, or 'b' to "
                        "return."
                     << endl;
                for (;;) {
                  //cin.clear();
                  //cin.ignore(numeric_limits<streamsize>::max(), '\n');
                  cin >> selection;
                  if (selection == 'p') {
                    selectedLoop->playAudioLoop(
                        selectedLoop->getdefaultAudio());
                  } else if (selection == 'b') {
                    break;
                  } else {
                    cout << "Invalid input, please try again." << endl;
                  }
                }
              }
            }
          }
        } else if (selection == 'w') {  // write loops
          continueSubSelection = true;
          string* tempName = new string;
          Loop* loopToWrite = nullptr;
          while (continueSubSelection) {
            cout << "Type and enter: 'y' to proceed with writing loop or 'b' "
                    "to return to loop menu."
                 << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> selection;
            if (selection == 'y') {
              library.getLoopNames();
              cout << "Please enter the name of the loop you would like to "
                      "write "
                      "to hard drive."
                   << endl;
              cin.clear();
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cin >> *tempName;
              loopToWrite = library.selectLoop(*tempName);
              if (loopToWrite == nullptr) {
                cout << "Loop '" << *tempName
                     << "' does not exist. Please try again." << endl;
              } else {
                if (loopToWrite->writeToFile()) {
                  cout << "Successfully wrote to file. Returning to loop menu."
                       << endl;
                  delete tempName;
                  delete loopToWrite;
                  continueSubSelection = false;
                } else {
                  cout << "Something went wrong, please try again." << endl;
                }
              }
            } else if (selection == 'b') {
              delete tempName;
              delete loopToWrite;
              continueSubSelection = false;
            } else {
              cout << "Invalid selection, please try again." << endl;
            }
          }
        }

        else if (selection == 'b') {
          continueMainSelection = false;
        }

        else {
          cout << "Invalid selection, please try again." << endl;
        }
      }
      // Quit
    } else if (selection == 'q') {  // quit
      while (true) {
        cout << "Are you sure you want to quit? All loops unwritten to file "
                "shall be lost.\nType and enter 'y' to confirm or 'b' to go "
                "back."
             << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> selection;
        if (selection == 'y') {
          cout << "Okay, goodbye!" << endl;
          continueProgram = false;
          break;
        } else if (selection == 'b') {
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