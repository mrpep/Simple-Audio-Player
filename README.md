# Simple-Audio-Player
A C++ minimal audio player based on wxWidgets, libsndfile and Portaudio.

Example showing how to code a UI in wxWidgets with multithreading support, read an audio file with libsndfile and play it through Portaudio.
It shows the progress of the song in real time and allows the user to jump to any part of it by clicking on a bar. Playback can be paused, volume changed and playlists created.

It supports WAV, FLAC and OGG formats. MP3 is not supported by libsndfile.

Some missing features that could be added in the future:
- More care when dealing with thread communication. Use of mutexs, conditions...
- Add more functionalities to playlist:
  - Possibility of removing audios.
  - Randomize order, loop playlist, etc...
- Add a MP3 decoder.

Binary is supplied for Windows 64 bit, but the code can be compiled in other platforms, although it will require downloading the libraries and linking them in a project.
