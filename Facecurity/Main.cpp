#include "FaceRecognizer.h"
#include "FaceSetup.h"
#include <iostream>
using namespace std;

int main(){
	char option;
	string username;
	FaceSetup *faceSetup = new FaceSetup();

	cout << "=======================\nWelcome to Facecurity\n=======================" << endl;
	cout << " Choose one of the following options:\n 1 - Setup\n 2 - Recognizer" << endl;

	cin >> option;
	switch (option){
	case '1':
	case 's':
		cout << "Please, follow the next steps to properly configure your face detection.\n";
		cout << "Make sure that you are in a place with good light conditions and try to have your face centered on the screen.\n";
		cout << "First, insert your username: ";
		cin >> username;
		faceSetup->startSetup(username);
		break;
	case '2':
	case 'r':
		startRecognition();
		break;
	};
}