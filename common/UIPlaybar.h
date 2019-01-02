//UIPlaybar.h
//Code written by Leonardo Pepino - 2019

#ifndef UIPLAYBAR_H
	#define UIPLAYBAR_H
	#pragma once

	#include "wx/wxprec.h"
	#include "wx/dcbuffer.h"

	#ifndef  WX_PRECOMP
		#include "wx/wx.h"
	#endif

	static const long ID_PLAYBARCLICK = 3000;

	class UIPlaybar : public wxPanel {

	public:
		UIPlaybar(wxWindow* panel, wxWindowID PlaybarID, wxSize PlaybarSize);
		void SetRectanglePos(double pos);
		double GetRectanglePos();
	private:
		wxWindow * m_panel;
		//wxPaintDC * m_dc;
		wxBufferedDC *m_dc;
		wxSize m_size;
		double m_audioposition;
		int posx;
		void drawWidget(wxPaintEvent& evt);
		void onSize(wxSizeEvent& evt);
		void onClick(wxMouseEvent& evt);
	};

#endif
