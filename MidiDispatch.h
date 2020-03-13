#ifndef MIDI_DISPATCH_H
#define MIDI_DISPATCH_H

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <chrono>
#include <condition_variable>

#include "RtMidi.h"
#include "midi.h"

namespace capstone {
    using std::chrono::milliseconds;
    using std::chrono::high_resolution_clock;
    using std::chrono::time_point;
    using std::priority_queue;

    typedef high_resolution_clock::time_point time_t;

    typedef struct {
        time_t time;
        std::vector<midi::MidiMessage> msg;
    } DispatchItem;

    class MidiDispatch {
    public:
        MidiDispatch(unsigned int port);
        ~MidiDispatch();
        void start();
        void stop();
        void forceStop();

        void enqueue(time_t time, midi::MidiMessage message);
        void enqueue(time_t time, std::initializer_list<midi::MidiMessage> messages);
        
    private:
        std::atomic<bool> running;
        std::atomic<bool> flush;
        std::atomic<bool> topDirty;

        milliseconds safetyTime;
        RtMidiOut* midiOut;

        std::mutex queueMutex;
        std::priority_queue<DispatchItem> queue;
        
        DispatchItem queueTop;
        std::thread dispatchThread;
        
        void dispatchLoop();
        void dispatch(DispatchItem item);
        DispatchItem getTop();
    };
}

#endif
