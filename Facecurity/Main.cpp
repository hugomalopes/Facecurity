#include "FaceRecognizer.h"
#include <iostream>
using namespace std;

int main(){
	char option;

	cout << "=======================\nWelcome to Facecurity\n=======================" << endl;
	cout << " Choose one of the following options:\n 1 - Setup\n 2 - Recognizer" << endl;

	cin >> option;
	switch (option){
	case '1':
	case 's':
		cout << "Open Setup" << endl; //TODO
		break;
	case '2':
	case 'r':
		startRecognition();
		break;
	};
}