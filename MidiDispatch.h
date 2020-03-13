#ifndef MIDI_DISPATCH_H
#define MIDI_DISPATCH_H

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <chrono>

#include "RtMidi.h"
#include "midi.h"

using std::chrono::microseconds;
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::time_point;
using std::priority_queue;

namespace capstone {
    typedef high_resolution_clock::time_point time_t;

    typedef struct {
        time_t time;
        std::vector<midi::MidiMessage> msg;
    } DispatchItem;

    class MidiDispatch {
    public:
        MidiDispatch(microseconds minDelta, unsigned int port);
        ~MidiDispatch();
        void start();
        void stop();
        void forceStop();

        void enqueue(time_t time, midi::MidiMessage message);
        void enqueue(time_t time, std::initializer_list<midi::MidiMessage> messages);
        
    private:
        bool running;
        bool flush;
        microseconds minDelta;
        RtMidiOut* midiOut;
        std::mutex queueMutex;
        std::priority_queue<DispatchItem> queue;
        DispatchItem queueTop;
        std::atomic<bool> queueDirty;
        std::thread dispatchThread;
        
        void dispatchLoop();
        void dispatch(DispatchItem item);
        DispatchItem getTop();
    };
}

#endif
