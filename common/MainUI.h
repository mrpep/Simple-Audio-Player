//MainUI.h
//Code written by Leonardo Pepino - 2019

#ifndef MAINUI_H
	#define MAINUI_H
	#pragma once
	
	#include "resource.h"
	#include "wx/wxprec.h"
	#include "wx/listbox.h"
	#include "wx/artprov.h"

	#ifndef WX_PRECOMP
	#include "wx/wx.h"
	#endif
	
	#include "UIPlaybar.h"

	#include <string>

	#include <sndfile.h>
	#include "AudioIO.h"
	#include <memory>

	//Unique IDS:
	static const int ID_PLAYLIST = 1005;
	static const int ID_PLAYBAR = 1006;
	static const int ID_LOADFILES = 1007;
	static const int ID_PLAY = 1008;
	static const int ID_SLMIX = 1009;

	class MainUI : public wxFrame {

	public:
		MainUI(const wxString& title);
		virtual ~MainUI();
		void LayoutUI();
		void SetReadPos();
		bool Play_Status = 0;

	protected:
		wxPanel * m_Panel;

	private:
		//UI Related:
		wxSlider * slMix;
		wxListBox * playlist;
		wxBitmapButton * btnLoad;
		wxButton * btnPlay;
		UIPlaybar * playBar;
		//Data Related:
		wxArrayString m_audioPaths;
		wxArrayString m_audioNames;
		//Audio Related:
		SNDFILE* m_audioFile;
		SF_INFO m_audioInfo;
		float m_audioArray[BUFFER_SIZE];
		sf_count_t count;
		AudioIO* m_audioPlayer;
		wxArrayString AudioDevices;

		float* processedaudio;
		int m_referencecounter = 0;
		int m_readhead = 0;
		int ListPos = 0;

		//Event Related:
		void onLoadFile(wxCommandEvent& evt);
		void onPlay(wxCommandEvent& evt);
		void onPlaylistSelect(wxCommandEvent& evt);
		void OnFinishPlayback(wxCommandEvent& evt);
		void SetPlaybarPos(wxCommandEvent& evt);
		void SetPlaybarAudioPos(wxCommandEvent& evt);
		void OnSlMix(wxCommandEvent& evt);
		void SelectAudio(int selected);
	};

#endif
