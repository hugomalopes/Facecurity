#include "FaceRecognizer.h"
#include "FaceLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace cv;
using namespace std;

void startRecognition(){
	const string lbp_file = "lbpcascade_frontalface.xml";
	const int deviceId = 0;
	FaceLoader *faceLoader = new FaceLoader();
	unordered_map<int, string> namesMap;

	// Read in the data (fails if no valid input filename is given, but you'll get an error message):
	try {
		faceLoader->readCSV();
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << "file" << "\". Reason: " << e.msg << endl; //mudar depois
		exit(1);
	}
	namesMap = faceLoader->getNamesMap();
	// Get the height from the first image.
	// Used to reshape the incoming faces this size
	int im_width = faceLoader->getImages()[0].cols;
	int im_height = faceLoader->getImages()[0].rows;
	
	// Create a FaceRecognizer and train it on the given images
	Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
	model->train(faceLoader->getImages(), faceLoader->getIdentifiers());
	
	delete faceLoader;
	// Create a Cascade Classifier using the xml file specifed.
	CascadeClassifier classifier;
	classifier.load(lbp_file);

	// Get a handle to the Video device:
	VideoCapture cap(deviceId);
	if (!cap.isOpened()) {
		cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
		return;
	}
	// Holds the current frame from the webcam:
	Mat frame;
	for (;;) {
		cap >> frame;
		// Clone the current frame:
		Mat original = frame.clone();
		// Convert the current frame to grayscale:
		Mat gray;
		cvtColor(original, gray, CV_BGR2GRAY);
		// Find the faces in the frame:
		vector< Rect_<int> > faces;
		classifier.detectMultiScale(gray, faces);
		// Checks if it's only one person at the image
		Mat face;
		if (faces.size() > 1)
			putText(original, "ONE PERSON AT A TIME, PLEASE", Point(180, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 2.0);
		else if(faces.size() == 1){
			// Crop the face from the image
			face = gray(faces[0]);
			// Resizing the face is necessary for Fisherfaces algotithm
			Mat face_resized;
			cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
			// Now perform the prediction
			int prediction = model->predict(face_resized);
			// Draw a green rectangle around the detected face
			rectangle(original, faces[0], CV_RGB(255, 255, 255), 1);
			// Create the text we will annotate the box with
			string box_text = namesMap.at(prediction);
			// Calculate the position for annotated text
			int pos_x = std::max(faces[0].tl().x - 10, 0);
			int pos_y = std::max(faces[0].tl().y - 10, 0);
			// And now put it into the image:
			putText(original, box_text, Point(pos_x+75, pos_y-50), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255, 255, 255), 1.5);
		}
		// Show the result:
		imshow("face_recognizer", original);
		// And display it:
		char key = (char)waitKey(10);
		// Exit this loop on escape:
		if (key == 27)
			break;
	}
}