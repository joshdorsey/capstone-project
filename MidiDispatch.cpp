#include "MidiDispatch.h"

namespace capstone {
	bool operator<(const DispatchItem& a, const DispatchItem& b) {
		return a.time > b.time;
	}

	MidiDispatch::MidiDispatch(microseconds minDelta, unsigned int port) : minDelta(minDelta) {
		midiOut = new RtMidiOut();
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
		std::lock_guard<std::mutex> lock(queueMutex);
		DispatchItem item = { time, { message } };
		queue.push(item);
		queueDirty = true;
	}

	void MidiDispatch::enqueue(time_t time, std::initializer_list<midi::MidiMessage> messages) {
		std::lock_guard<std::mutex> lock(queueMutex);
		DispatchItem item = { time, messages };
		queue.push(item);
		queueDirty = true;
	}

	void MidiDispatch::dispatchLoop() {
		std::lock_guard<std::mutex> lock(queueMutex);
		while (running || !queue.empty()) {
			if (queue.empty()) {
				std::this_thread::sleep_for(minDelta);
				continue;
			}

			if (flush) {
				while (!queue.empty()) queue.pop();
				continue;
			}

			time_t startDispatch = high_resolution_clock::now();
			DispatchItem top = this->getTop();
			while (!queue.empty() && startDispatch > top.time) {
				dispatch(top);

				queue.pop();
				queueDirty = true;

				if (!queue.empty()) {
					top = this->getTop();
				}
			}

			time_t endDispatch = high_resolution_clock::now();
			std::this_thread::sleep_for(minDelta - (endDispatch - startDispatch));
		}
	}

	void MidiDispatch::dispatch(DispatchItem item) {
		for (midi::MidiMessage msg : item.msg) {
			std::vector<uint8_t> out = { msg.status };

			if (msg.data1 < 0x80) {
				out.push_back(msg.data1);

				if (msg.data2 < 0x80) {
					out.push_back(msg.data2);
				}
			}

			midiOut->sendMessage(&out);
		}
	}

	DispatchItem MidiDispatch::getTop() {
		if (queueDirty) {
			queueTop = queue.top();

			queueDirty = false;
		}

		return queueTop;
	}
};