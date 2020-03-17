// STD stuff
#include <memory>

// wxWidgets stuff
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

// MIDI stuff
#include <RtMidi.h>
#include <midi/MidiDispatch.h>
#include <chrono>

using std::chrono::high_resolution_clock;
using namespace std::chrono_literals;
using std::unique_ptr;
using std::make_unique;
using midi::MidiDispatch;

enum {
	ID_Hello = 1
};

class TestFrame : public wxFrame {
public:
	TestFrame() : wxFrame(NULL, wxID_ANY, "Hello World") {
		dispatch = make_unique<MidiDispatch>(6);
		dispatch->start();

		wxMenu* fileMenu = new wxMenu();
		fileMenu->Append(ID_Hello, "&Hello...\tCtrl-M", "Send some notes");
		fileMenu->AppendSeparator();
		fileMenu->Append(wxID_EXIT);

		wxMenu* helpMenu = new wxMenu();
		helpMenu->Append(wxID_ABOUT);

		wxMenuBar* menuBar = new wxMenuBar();
		menuBar->Append(fileMenu, "F&ile");
		menuBar->Append(helpMenu, "&Help");

		SetMenuBar(menuBar);

		CreateStatusBar();
		SetStatusText("I want to die!!");

		Bind(wxEVT_MENU, &TestFrame::OnHello, this, ID_Hello);
		Bind(wxEVT_MENU, &TestFrame::OnExit, this, wxID_EXIT);
		Bind(wxEVT_MENU, &TestFrame::OnAbout, this, wxID_ABOUT);
	}

private:
	unique_ptr<MidiDispatch> dispatch;

	void OnHello(wxCommandEvent& event) {
		auto now = high_resolution_clock::now();
		dispatch->enqueue(now + 1s, midi::noteOn(1, 80, 64));
		dispatch->enqueue(now + 2s, midi::noteOff(1, 80));
	}

	void OnExit(wxCommandEvent& event) {
		dispatch->stop();
		Close();
	}

	void OnAbout(wxCommandEvent& event) {
		wxMessageBox("Hello world blah blah", "About this program yeye", wxOK | wxICON_INFORMATION);
	}
};

class UiTest : public wxApp {
public:
	virtual bool OnInit() {
		TestFrame* frame = new TestFrame();
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(UiTest);