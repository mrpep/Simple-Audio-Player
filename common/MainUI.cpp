//MainUI.cpp
//Code written by Leonardo Pepino - 2019

#include "MainUI.h"

MainUI::MainUI(const wxString & title) : wxFrame(NULL,wxID_ANY,title,wxPoint(0,0),wxGetDisplaySize()) {

	LayoutUI();
	m_referencecounter = 0;
}

MainUI::~MainUI() {

}

void MainUI::LayoutUI() {

	m_Panel = new wxPanel(this);

	//Sizers for UI Layout:
	wxBoxSizer * mainBox = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * playbackBox = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * fileBox = new wxBoxSizer(wxVERTICAL);

	//Control for level:
	slMix = new wxSlider(m_Panel,ID_SLMIX,0,0,100,wxDefaultPosition,wxDefaultSize,wxSL_VERTICAL);

	//Playlist:
	playlist = new wxListBox(m_Panel, ID_PLAYLIST, wxDefaultPosition, wxDefaultSize);
	
	//Playback Controls:
	//Native Icons:
	wxBitmap iconLoad = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_BUTTON);
	btnLoad = new wxBitmapButton(m_Panel,ID_LOADFILES,iconLoad,wxDefaultPosition,wxSize(30,30));
	btnPlay = new wxButton(m_Panel, ID_PLAY, _(">"), wxDefaultPosition, wxSize(30, 30));
	playBar = new UIPlaybar(m_Panel, ID_PLAYBAR, wxDefaultSize);

	mainBox->Add(slMix, 0, wxEXPAND | wxALL, 10);
	playbackBox->Add(btnLoad, 0);
	playbackBox->Add(btnPlay, 0);
	playbackBox->Add(playBar, 1, wxEXPAND);
	fileBox->Add(playlist, 1, wxEXPAND | wxALL, 10);
	fileBox->Add(playbackBox,0,wxEXPAND|wxALL,10);
	mainBox->Add(fileBox,2,wxEXPAND);

	m_Panel->SetSizerAndFit(mainBox);

	btnLoad->Bind(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainUI::onLoadFile),this);
	btnPlay->Bind(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainUI::onPlay), this);
	playlist->Bind(wxEVT_LISTBOX_DCLICK, wxCommandEventHandler(MainUI::onPlaylistSelect), this);
	Connect(ID_REFRESHREADPOS, wxEVT_ANY, wxCommandEventHandler(MainUI::SetPlaybarAudioPos));
	Connect(ID_EOF, wxEVT_ANY, wxCommandEventHandler(MainUI::OnFinishPlayback));
	slMix->Bind(wxEVT_SLIDER, wxCommandEventHandler(MainUI::OnSlMix),this);
}

void MainUI::onLoadFile(wxCommandEvent& evt) {

	wxFileDialog* LoadDialog = new wxFileDialog(this, _("Select audio files"), wxEmptyString, wxEmptyString,
		_("Audio Files (*.wav;*.mp3;*.ogg;*.flac)|*.wav;*.ogg;*.flac"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
	if (LoadDialog->ShowModal() == wxID_CANCEL) {
		return;
	}

	wxArrayString tempPaths;
	wxArrayString tempNames;
	LoadDialog->GetPaths(tempPaths); //Stores full paths loaded
	for (int i = 0; i < tempPaths.size(); i++) {
		tempNames.Add(tempPaths[i].AfterLast('\\')); //Stores just the names for adding playlist display
		m_audioNames.Add(tempPaths[i].AfterLast('\\')); //Whole list of names
		m_audioPaths.Add(tempPaths[i]); //Whole list of file paths
	}
	playlist->Append(tempNames);

}

void MainUI::onPlay(wxCommandEvent& evt) {
	//Toggle play status
	Play_Status = !Play_Status;
	if (Play_Status) {
		btnPlay->SetLabelText("||");
	} else {
		btnPlay->SetLabelText(">");
	}

}

void MainUI::onPlaylistSelect(wxCommandEvent& evt) {
	int selected = playlist->GetSelection();
	ListPos = selected;
	SelectAudio(selected);
}

void MainUI::SetPlaybarPos(wxCommandEvent& evt) {

	int pos = evt.GetInt();
	double percentage = static_cast<double>(pos) / static_cast<double>(2*m_audioInfo.frames);
	playBar->SetRectanglePos(percentage);

}

void MainUI::SetPlaybarAudioPos(wxCommandEvent& evt) {

	int pos = evt.GetInt();
	double percentage = static_cast<double>(pos) / static_cast<double>(m_audioInfo.frames);
	playBar->SetRectanglePos(percentage);

}

void MainUI::SetReadPos() {

	double pos = playBar->GetRectanglePos();
	m_audioPlayer->SetPosition(static_cast<int>(pos*(m_audioInfo.frames)));
}

void MainUI::OnSlMix(wxCommandEvent& evt) {
	double pos = slMix->GetValue();
	pos = (slMix->GetMax() - pos) / slMix->GetMax();
	m_audioPlayer->SetMainGain(pos);
}

void MainUI::OnFinishPlayback(wxCommandEvent& evt) {
	int NFiles = playlist->GetCount();
	
	if (ListPos + 1 < NFiles) {
		SelectAudio(ListPos + 1);
		playlist->Select(ListPos + 1);
		ListPos++;
	}
}

void MainUI::SelectAudio(int selected) {
	//Get the selection audio path and convert to char:
	std::string path = std::string(m_audioPaths[selected].mb_str());
	const char* pathc = path.c_str();

	//Check if I have started a thread:
	if (m_referencecounter > 0) {
		m_audioPlayer->playbackStatus = 0;
		wxMilliSleep(20);
		sf_close(m_audioFile);
	}

	m_referencecounter++;
	//Open the audio file with libsndfile:
	m_audioInfo.format = 0;
	m_audioFile = sf_open(pathc, SFM_READ, &m_audioInfo);

	//Check if something went wrong:
	if (m_audioFile == NULL) {
		wxMessageDialog MsgBox(this,
			_("Audio file could not be opened."),
			_T("File Error"), wxOK | wxICON_ERROR | wxSTAY_ON_TOP);
		MsgBox.ShowModal();
	} else {
		//If everything is OK:
		int nframes = m_audioInfo.frames;
		m_readhead = 0;

		m_audioPlayer = new AudioIO(m_audioFile, &m_audioInfo,this, &Play_Status, &m_readhead);
		m_audioPlayer->SetDevice(m_audioInfo.frames, -1, m_audioInfo.channels, m_audioInfo.samplerate);
		m_audioPlayer->Create();
		m_audioPlayer->Run();

		double pos = slMix->GetValue();
		pos = (slMix->GetMax() - pos) / slMix->GetMax();
		m_audioPlayer->SetMainGain(pos);
		
		Play_Status = 1;
		btnPlay->SetLabelText(_("||"));
	}
}
