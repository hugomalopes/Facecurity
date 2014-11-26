Facecurity
==========

A powerful tool to authorize the right person to login using face recognition.


Compile and run in Linux:
Change "\" to "/" in .ext file
g++ `pkg-config --cflags opencv` -std=c++0x -o FaceRecognizer FaceRecognizer.cpp `pkg-config --libs opencv`
./FaceRecognizer
