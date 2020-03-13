#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include "config.h"
#include "timing.h"
#include "MidiDispatch.h"

using std::cout;
using std::cin;
using capstone::MidiDispatch;

using namespace std::chrono_literals;
using std::chrono::high_resolution_clock;

int main() {
    RtMidiOut* out = new RtMidiOut();
    
    cout << "Capstone Project (";
    cout << VERSION_MAJOR << "." << VERSION_MINOR;
    cout << ")\n";

    cout << "The following MIDI Outputs are available:\n";

    for (unsigned int i = 0; i < out->getPortCount(); i++) {
        cout << i << ")\t" << out->getPortName(i) << "\n";
    }

    unsigned int port;
    cout << "> ";
    cin >> port;

    MidiDispatch dispatch(port);
    dispatch.start();

    capstone::time_t start = high_resolution_clock::now();
    dispatch.enqueue(start + 1s, {
        midi::noteOn(1, 80, 64),
        midi::noteOn(1, 84, 64),
        midi::noteOn(1, 87, 64),
    });

    dispatch.enqueue(start + 2s, {
        midi::noteOff(1, 80),
        midi::noteOff(1, 84),
        midi::noteOff(1, 87),
        midi::noteOn(1, 78, 64),
        midi::noteOn(1, 82, 64),
        midi::noteOn(1, 85, 64),
    });

    dispatch.enqueue(start + 3s, {
        midi::noteOff(1, 78),
        midi::noteOff(1, 82),
        midi::noteOff(1, 85),
        midi::noteOn(1, 77, 64),
        midi::noteOn(1, 81, 64),
        midi::noteOn(1, 84, 64),
    });

    dispatch.enqueue(start + 4s, {
        midi::noteOff(1, 77),
        midi::noteOff(1, 81),
        midi::noteOff(1, 84),
    });

    uint8_t note = 40;

    start += 4s;
    for (unsigned int i = 0; i < 20; i++) {
        note += 2;
        dispatch.enqueue(start + (500ms * i), {
            midi::noteOn(1, note, 64),
            midi::noteOff(1, note - 2),
        });
    }

    dispatch.enqueue(start + (500ms * 20), midi::noteOff(1, note));

    dispatch.stop();

    return 0;
}
