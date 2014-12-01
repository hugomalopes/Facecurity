#include "FaceSetup.h"
#include "FaceLoader.h"
	
void startSetup(string username){
	const string lbp_file = "lbpcascade_frontalface.xml";
	const int deviceId = 0;
	// Used to reshape the faces this size
	const int im_width = 240;
	const int im_height = 240;

	const int NUM_PHOTOS = 10;
	int photosTaken = 1;
	bool takePhoto = false;
	int id;
	FaceLoader *faceLoader = new FaceLoader();
	ofstream myfile;
	
	try {
		id = faceLoader->readCSV() + 1;
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << "file" << "\". Reason: " << e.msg << endl; //mudar depois
		exit(1);
	}

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
		Mat face, face_resized;
		if (faces.size() > 1)
			putText(original, "ONE PERSON AT A TIME, PLEASE", Point(180, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 2);
		else if (faces.size() == 1){
			// Crop the face from the image
			face = gray(faces[0]);
			// Resizing the face is necessary for Fisherfaces algotithm
			cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
			rectangle(original, faces[0], CV_RGB(255, 255, 255), 1);

			if (takePhoto && photosTaken <= NUM_PHOTOS && !face_resized.empty()){
				string filename = username + to_string(photosTaken);
				imwrite("faces/" + filename + ".jpg", face_resized); // save as jpg in faces folder
				string output = "faces/" + filename + ".jpg;" + to_string(id) + ";" + username;
				putText(original, "TAKING PHOTO " + to_string(photosTaken), Point(220, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 2);
				photosTaken++;

				myfile << output << endl;
			}
			else if (photosTaken > NUM_PHOTOS){
				myfile << endl;
				myfile.close();
				return;
			}
		}
		// Show the image from cam
		imshow("face_recognizer", original);

		char key = (char)waitKey(10);
		// Exit this loop on escape:
		if (key == 27)
			break;
		if (key == 8){
			takePhoto = true;
			myfile.open("faces_csv.ext", ios_base::app);
		}
	}
};