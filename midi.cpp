#include "midi.h"

namespace midi {
    MidiMessage noteOn(uint8_t channel, uint8_t note, uint8_t vel) {
        // Limit channel to 4 bits
        channel &= 0x0F;
        // Limit note to 7 bits
        note &= 0x7F;
        // Limit velocity to 7 bits
        vel &= 0x7F;
        return { STATUS_BYTE(NOTE_ON, channel), note, vel };
    }

    MidiMessage noteOff(uint8_t channel, uint8_t note) {
        // Limit channel to 4 bits
        channel &= 0x0F;
        // Limit note to 7 bits
        note &= 0x7F;
        return { STATUS_BYTE(NOTE_OFF, channel), note, 0x00 };
    }

    MidiMessage noteOff(uint8_t channel, uint8_t note, uint8_t vel) {
        // Limit channel to 4 bits
        channel &= 0x0F;
        // Limit note to 7 bits
        note &= 0x7F;
        // Limit velocity to 7 bits
        vel &= 0x7F;
        return { STATUS_BYTE(NOTE_OFF, channel), note, vel };
    }
}