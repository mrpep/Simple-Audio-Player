//AudioIO.h
//Code written by Leonardo Pepino - 2019

#ifndef AudioIO_H
	#define AudioIO_H
	#pragma once
	
	#include <portaudio.h>
	#include<sndfile.h>

	#ifndef WX_PRECOMP
		#include "wx/wx.h"
	#endif
	
	#include "wx/thread.h"
	#include <memory>

static const long ID_REFRESHREADPOS = 2000;
static const long ID_EOF = 2001;

enum Settings {
	BUFFER_SIZE = 512
};

class AudioIO : public wxThread {

public:

	AudioIO(SNDFILE* audiofile, SF_INFO* audioinfo, wxFrame* frame, bool* status, int* readhead);
	~AudioIO();

	int playbackStatus;

	//wxArrayString GetDeviceList();
	void SetDevice(int frames, int ndev = -1, int nchannels = 2, int fs = 44100);
	//void SetFile(SNDFILE* audioFile,SF_INFO audioInfo);
	void TerminateStream();
	void SetMainGain(double gain);
	void SetPosition(int pos);
	//void WriteBuffer(float* towrite);
	//void AllocateBuffer(int nframes);
	virtual void* Entry();

	//bool READING = 1;

protected:
private:

	//static const int BUFFER_SIZE = 2048;
	PaError err;
	PaStreamParameters outputParameters;
	//unsigned long readPos;
	SNDFILE* m_audioFile;
	SF_INFO* m_audioInfo;
	PaStream* stream;
	sf_count_t m_count;
	//sf_count_t m_count;
	
	//int m_writeHead;
	int m_nframes;
	int iters;

	bool* m_playstatus;
	float* m_audio;
	int* m_readHead;

	double m_mixGain = 1.0;
	
	//bool READING_BUFFER = 0;
	//bool WRITE_BUFFER = 1;
	
	//wxMutex* m_mutex;
	//wxCondition* m_reading; wxCondition* m_writing;
	wxFrame* m_mainframe;

	static int paCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

	int CallbackMethod(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags);
};
	
#endif
