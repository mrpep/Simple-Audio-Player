//SimpleAudioPlayer.h
//Code written by Leonardo Pepino - 2019

#ifndef SIMPLEAUDIOPLAYER_H
	#define SIMPLEAUDIOPLAYER_H
	#pragma once

	#include "wx/wxprec.h"

	#ifndef  WX_PRECOMP
	#include "wx/wx.h"
	#endif
	
	#include "MainUI.h"

	class SimpleAudioPlayer : public wxApp {

		protected:
			virtual bool OnInit();
			virtual int OnExit();
			virtual int FilterEvent(wxEvent& event);
			MainUI *m_pMainFrame;
	};

	wxIMPLEMENT_APP(SimpleAudioPlayer);

#endif
