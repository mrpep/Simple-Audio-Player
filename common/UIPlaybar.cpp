//UIPlaybar.cpp
//Code written by Leonardo Pepino - 2019

#include "UIPlaybar.h"

UIPlaybar::UIPlaybar(wxWindow* panel, wxWindowID PlaybarID, wxSize PlaybarSize) {
	
	m_panel = panel;	
	Create(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
	m_audioposition = 0.0;
	m_size = PlaybarSize;
	
	Bind(wxEVT_PAINT, wxPaintEventHandler(UIPlaybar::drawWidget),this);
	Bind(wxEVT_SIZE, wxSizeEventHandler(UIPlaybar::onSize), this);
	Bind(wxEVT_LEFT_UP, wxMouseEventHandler(UIPlaybar::onClick), this);
}

void UIPlaybar::drawWidget(wxPaintEvent& evt) {

	wxPaintDC dc(this);
	dc.SetBrush(*wxBLACK_BRUSH);
	m_size = this->GetSize();
	int pixelsaudio = static_cast<int>(m_size.GetX()*m_audioposition);
	dc.DrawRectangle(0, 0, pixelsaudio, 30);

}

void UIPlaybar::onSize(wxSizeEvent& evt) {

	this->Refresh();

}

void UIPlaybar::onClick(wxMouseEvent& evt) {
	m_size = this->GetSize();
	posx = evt.GetX();
	m_audioposition = static_cast<double>(posx)/static_cast<double>(m_size.GetX());
	this->Refresh();

	wxCommandEvent *clickevt = new wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, ID_PLAYBARCLICK);
	wxTheApp->QueueEvent(clickevt);
}


void UIPlaybar::SetRectanglePos(double pos) {

	m_audioposition = pos;
	this->Refresh();

}

double UIPlaybar::GetRectanglePos() {

	return m_audioposition;
}