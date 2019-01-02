//SimpleAudioPlayer.cpp
//Code written by Leonardo Pepino - 2019

#include "SimpleAudioPlayer.h"

bool SimpleAudioPlayer::OnInit() {

	// Create GUIFrame

	m_pMainFrame = new MainUI(_("Simple Audio Player"));

	m_pMainFrame->Show(true);
	m_pMainFrame->Update();
	SetTopWindow(m_pMainFrame);

	return true;

}

int SimpleAudioPlayer::OnExit() {

	return wxApp::OnExit();

}

int SimpleAudioPlayer::FilterEvent(wxEvent& evt) {
	if (evt.GetId() == ID_PLAYBARCLICK) {
		m_pMainFrame->SetReadPos();
		return Event_Processed;
	}
	return Event_Skip;
}