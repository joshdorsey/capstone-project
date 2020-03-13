#include <iostream>
#include "RtMidi.h"
#include "config.h"

using std::cout;

int main() {
    RtMidiOut* out = new RtMidiOut();
    
    cout << "Capstone Project (" << VERSION_MAJOR << "." << VERSION_MINOR << ")\n";

    cout << "The following MIDI Outputs are available:\n";

    for (size_t i = 0; i < out->getPortCount(); i++) {
        cout << i << ")\t" << out->getPortName(i) << "\n";
    }

    return 0;
}
