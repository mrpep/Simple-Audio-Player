//AudioIO.cpp
//Code written by Leonardo Pepino - 2019

#include "AudioIO.h"

//Audio IO happens in a separate thread
AudioIO::AudioIO(SNDFILE* audiofile, SF_INFO* audioinfo, wxFrame* frame, bool* playstatus, int* readhead):wxThread() {
	//Portaudio is initialized:
	m_readHead = readhead;
	m_audioFile = audiofile;
	m_audioInfo = audioinfo;
	m_playstatus = playstatus;
	m_mainframe = frame;
	iters = 0;
	err = Pa_Initialize();
}

AudioIO::~AudioIO() {
	//Portaudio is terminated:
	err = Pa_Terminate();
}

void AudioIO::SetDevice(int frames, int ndevice, int nchannels,int fs) {

	m_nframes = frames;
	//Output device is set and a stream is opened, ndevice = -1 means default.
	if (ndevice == -1) {
		outputParameters.device = Pa_GetDefaultOutputDevice();
	} else {
		outputParameters.device = ndevice;
	}
	outputParameters.channelCount = nchannels;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
		&stream,
		NULL, /* no input */
		&outputParameters,
		fs,
		BUFFER_SIZE,
		NULL,      
		&AudioIO::paCallback,
		this);

}

int AudioIO::paCallback(const void *inputBuffer, void *outputBuffer,
	//This is the callback which is called when a buffer is required. It calls a class method.
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData) {

	//The class itself is passed as userData, being its methods and variables accesible from callback:
	return ((AudioIO*)userData)->CallbackMethod(inputBuffer, outputBuffer,
		framesPerBuffer,
		timeInfo,
		statusFlags);
}

int AudioIO::CallbackMethod(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags) {

	//Here the output buffer is filled.

	(void)timeInfo; /* Prevent unused variable warnings. */
	(void)statusFlags;
	(void)inputBuffer;

	if (*m_playstatus) {
		float *out = (float*)outputBuffer;
		//Set cursor at playback position
		sf_seek(m_audioFile, *m_readHead, SF_SEEK_SET);
		//Read a chunk from audio file:
		m_count = sf_read_float(m_audioFile, out, BUFFER_SIZE*m_audioInfo->channels);

		for (int i = 0; i < BUFFER_SIZE*m_audioInfo->channels; i++) {
			out[i] = out[i] * m_mixGain;
		}

		*m_readHead = *m_readHead + BUFFER_SIZE;
		//Each 10 iterations MainUI playbar is updated:
		if (iters % 10 == 0) {
			wxCommandEvent* evt = new wxCommandEvent(wxEVT_ANY, ID_REFRESHREADPOS);
			evt->SetInt(*m_readHead);
			m_mainframe->GetEventHandler()->AddPendingEvent(*evt);
		}

		iters++;

		//While EOF is not reached everything is OK
		if (m_count>0) {
			return paContinue;
		}
		//If EOF is reached Main Thread is notified and this thread stops:
		else {
			sf_close(m_audioFile);
			wxCommandEvent* evt = new wxCommandEvent(wxEVT_ANY, ID_EOF);
			m_mainframe->GetEventHandler()->AddPendingEvent(*evt);
			return paAbort;
		}
	}
	//If audio is paused, fill output buffer with 0. Other approaches could be sleeping Portaudio, but I found them
	//to cause glitches and undesirable results.
	else {
		Pa_StopStream(&stream);
		while (! *m_playstatus) {
			float *out = (float*)outputBuffer;
			for (int i = 0; i < m_audioInfo->channels * BUFFER_SIZE; i++) {
				out[i] = 0;
			}
			return paContinue;
		}
		//When is not paused anymore, stream restarts.
		Pa_StartStream(&stream);
		
		return paContinue;
	}
}

void AudioIO::TerminateStream() {
	err = Pa_StopStream(stream);
	err = Pa_CloseStream(stream);
}

//This is the function which is called when a wxThread runs:
void* AudioIO::Entry() {
	playbackStatus = 1;
	*m_readHead = 0;
	err = Pa_StartStream(stream);
	//We don't want the thread to finish while PortAudio is working so here it is paused.
	//It sleeps 5 ms in each iteration so that it doesn't slow down main thread.
	while (playbackStatus) {
		Sleep(5);
	}
	//Once everything is done the stream is terminated and a null pointer return indicates wxWidgets to finish the thread.
	TerminateStream();
	return NULL;
}

void AudioIO::SetMainGain(double gain) {

	m_mixGain = gain;

}

void AudioIO::SetPosition(int pos) {

	//sf_count_t position = pos;
	*m_readHead = pos;

}