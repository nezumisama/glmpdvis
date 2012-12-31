# GlMPDVis - Music visualization for MPD using OpenGL.
## Description
This program draws a three-dimensional visualization (which currently is a spectrogram) of music played by the Music Player Daemon, using the OpenGL API.
It is currently at an experimental stage with (almost) no features.
## Building
Run make in the main directory (the one with this file). 
After successful build, the executable file should appear in the *src* directory.
There's currently no build tools used, they will be later.
If you happen to get a link error, it a missing dependency most probably.
## Dependencies
This is hopefully a full list:
* OpenGL (comes with your graphic drivers usually). Note you need OpenGL 1.5 or higher supported by the drivers.
* GLU (comes with OpenGL usually).
* GLFW
* GLEW
* FFTW3

