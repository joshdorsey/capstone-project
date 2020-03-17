#ifndef MIDI_H
#define MIDI_H

#include <cstdint> // uint8_t

#define NOTE_OFF   0x80
#define NOTE_ON    0x90
#define KEY_PRESS  0xA0
#define CCHANGE    0xB0
#define PCHANGE    0xC0
#define CHAN_PRESS 0xD0
#define PITCH_BEND 0xE0

#define STATUS_BYTE(type, channel) (uint8_t)(type + (channel & 0x0F))

namespace midi {
    typedef struct {
        uint8_t status;
        uint8_t data1;
        uint8_t data2;
    } MidiMessage;

    MidiMessage noteOn(uint8_t channel, uint8_t note, uint8_t vel);
    
    MidiMessage noteOff(uint8_t channel, uint8_t note);
    
    MidiMessage noteOff(uint8_t channel, uint8_t note, uint8_t vel);
};

#endif // midi.h
