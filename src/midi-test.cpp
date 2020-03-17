#include <iostream>
#include <cstdlib>
#include <RtMidi.h>
#include <midi/MidiDispatch.h>
#include <timing.h>
#include "config.h"

using std::cout;
using std::cin;
using midi::MidiDispatch;

using namespace std::chrono_literals;
using std::chrono::high_resolution_clock;

int main() {
    RtMidiOut* portInfo = new RtMidiOut();
    
    cout << "Capstone Project (";
    cout << VERSION_MAJOR << "." << VERSION_MINOR;
    cout << ")\n";

    cout << "The following MIDI Outputs are available:\n";

    for (unsigned int i = 0; i < portInfo->getPortCount(); i++) {
        cout << i << ")\t" << portInfo->getPortName(i) << "\n";
    }

    delete portInfo;

    unsigned int port;
    cout << "> ";
    cin >> port;

    MidiDispatch dispatch(port);
    dispatch.start();

    midi::time_t start = high_resolution_clock::now();
    uint8_t startNote = 60;
    uint8_t lastNote = startNote;
    auto noteLength = 100ms;
    uint8_t offset = 0;
    uint8_t noteNum = 25;
    int8_t stride = 9;

    for (unsigned int i = 0; i < noteNum; i++) {
        offset += stride;
        offset %= 19;
        
        dispatch.enqueue(start + (noteLength * i), {
            midi::noteOn(1, startNote + offset, 64),
            midi::noteOff(1, lastNote),
        });

        lastNote = startNote + offset;
    }

    dispatch.enqueue(start + (noteLength * noteNum), midi::noteOff(1, lastNote));

    dispatch.enqueue(start + (noteLength * noteNum) + 10ms, midi::noteOn(1, 100, 127));

    cout << "Playing Notes... ";
    dispatch.stop();
    cout << "done.\n";

    return 0;
}
