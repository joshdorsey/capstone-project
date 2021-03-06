#include "MidiDispatch.h"

namespace midi {
	using namespace std::chrono_literals;

	bool operator<(const DispatchItem& a, const DispatchItem& b) {
		return a.time > b.time;
	}

	MidiDispatch::MidiDispatch(unsigned int port) : safetyTime(1ms) {
		midiOut = make_unique<RtMidiOut>();
		midiOut->openPort(port);
		running = false;
		flush = false;
	}

	MidiDispatch::~MidiDispatch() {
		// When this falls out of scope, kill it
		this->forceStop();
	}

	void MidiDispatch::start() {
		running = true;
		dispatchThread = std::thread(&MidiDispatch::dispatchLoop, this);
	}

	void MidiDispatch::stop() {
		if (!dispatchThread.joinable()) {
			return;
		}

		running = false;
		dispatchThread.join();
	}

	void MidiDispatch::forceStop() {
		if (!dispatchThread.joinable()) {
			return;
		}

		running = false;
		flush = true;
		dispatchThread.join();
	}

	void MidiDispatch::enqueue(time_t time, midi::MidiMessage message) {
		DispatchItem item = { time, { message } };
		queueMutex.lock();
		queue.push(item);
		queueMutex.unlock();
	}

	void MidiDispatch::enqueue(time_t time, std::initializer_list<midi::MidiMessage> messages) {
		DispatchItem item = { time, messages };
		queueMutex.lock();
		queue.push(item);
		queueMutex.unlock();
	}

	void MidiDispatch::dispatchLoop() {
		bool queueEmpty = false;

		while (running || !queueEmpty) {
			queueMutex.lock();
			queueEmpty = queue.empty();
			queueMutex.unlock();

			if (queueEmpty) {
				std::this_thread::sleep_for(safetyTime);
				continue;
			}

			if (flush) {
				queueMutex.lock();
				while (!queue.empty()) queue.pop();
				queueMutex.unlock();
				flush = false;
				continue;
			}

			queueMutex.lock();
			DispatchItem top = queue.top();

			// Wait until we're to what's on top of the queue
			if (high_resolution_clock::now()+safetyTime > top.time) {
				// Spin until we need to dispatch
				while (high_resolution_clock::now() < top.time);

				dispatch(top);
				queue.pop();

				queueEmpty = queue.empty();

				if (!queueEmpty) {
					top = queue.top();
				}
			}

			queueEmpty = queue.empty();
			queueMutex.unlock();

			std::this_thread::yield();
		}
	}

	void MidiDispatch::dispatch(DispatchItem item) {
		for (midi::MidiMessage msg : item.msg) {
			if (msg.data1 < 0x80) {
				if (msg.data2 < 0x80) {
					midiOut->sendMessage((uint8_t*)(&msg), 3);
				} else {
					midiOut->sendMessage((uint8_t*)(&msg), 2);
				}
			}
			else {
				midiOut->sendMessage((uint8_t*)(&msg), 1);
			}
		}
	}
};