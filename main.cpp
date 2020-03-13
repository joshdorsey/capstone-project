#include <iostream>
#include "RtMidi.h"
#include "config.h"
#include "timing.h"
#include "MidiDispatch.h"

using std::cout;
using std::cin;
using capstone::MidiDispatch;
using std::chrono::seconds;
using std::chrono::milliseconds;

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

    MidiDispatch dispatch(microseconds(500), port);
    dispatch.start();

    timing::start("Building Queue");
    capstone::time_t now = high_resolution_clock::now();
    dispatch.enqueue(now, {
        midi::noteOn(1, 80, 64),
        midi::noteOn(1, 84, 64),
        midi::noteOn(1, 87, 64),
    });

    dispatch.enqueue(now + seconds(1), {
        midi::noteOff(1, 80),
        midi::noteOff(1, 84),
        midi::noteOff(1, 87),
        midi::noteOn(1, 78, 64),
        midi::noteOn(1, 82, 64),
        midi::noteOn(1, 85, 64),
    });

    dispatch.enqueue(now + seconds(2), {
        midi::noteOff(1, 78),
        midi::noteOff(1, 82),
        midi::noteOff(1, 85),
        midi::noteOn(1, 77, 64),
        midi::noteOn(1, 81, 64),
        midi::noteOn(1, 84, 64),
    });

    dispatch.enqueue(now + seconds(3), {
        midi::noteOff(1, 77),
        midi::noteOff(1, 81),
        midi::noteOff(1, 84),
    });

    uint8_t note = 40;

    now += seconds(3);
    for (unsigned int i = 0; i < 20; i++) {
        note += 2;
        dispatch.enqueue(now + (milliseconds(50) * i), {
            midi::noteOn(1, note, 64),
            midi::noteOff(1, note - 2),
        });
    }

    dispatch.enqueue(now + (milliseconds(50) * 20), midi::noteOff(1, note));
    timing::end("Building Queue");

    cout << "Playing notes...\n";

    timing::start("Waiting for Dispatch to Stop");
    dispatch.stop();
    timing::end("Waiting for Dispatch to Stop");

    timing::printSummary(cout);

    return 0;
}
