#include "FaceSetup.h"
#include "FaceLoader.h"

FaceSetup::FaceSetup() : faces_csv_file("faces_csv.ext"),
			  haar_profileface_file("haarcascade_profileface.xml"),
			  haar_smile_file("haarcascade_smile.xml"),
			  lbp_frontalface_file("lbpcascade_frontalface.xml"),
			  lbp_profileface_file("lbpcascade_profileface.xml"),
			  deviceId(0),
			  im_width(240),
			  im_height(240),
			  NUM_PHOTOS(10) {}

FaceSetup::~FaceSetup() {}

void FaceSetup::getBrightness(Mat& frame, double& brightness) {
	Mat temp_frame, color[3], lum;
	temp_frame = frame;

	split(temp_frame, color);

	color[0] = color[0] * 0.299;
	color[1] = color[1] * 0.587;
	color[2] = color[2] * 0.114;

	lum = color[0] + color [1] + color[2];

	Scalar color_sum = sum(lum);

	brightness = color_sum[0]/((pow(2,8)-1)*frame.rows * frame.cols) * 2; //-- percentage conversion factor
}

void FaceSetup::wait(VideoCapture cap) {
	// Holds the current frame from the webcam:
	Mat frame;
	for (;;) {
		cap >> frame;
		// Clone the current frame:
		Mat original = frame.clone();

		// Show the image from cam
		imshow("face_recognizer", original);

		char key = (char)waitKey(10);
		// Exit this loop on escape:
		if (key == 32)
			break;
	}
}

void FaceSetup::brightnessCheck(VideoCapture cap) {
	double brightness;

	// Holds the current frame from the webcam:
	Mat frame;
	for (;;) {
		cap >> frame;
		// Clone the current frame:
		Mat original = frame.clone();

		getBrightness(frame, brightness);
		putText(original, std::to_string(brightness), Point(180, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 2);

		// Show the image from cam
		imshow("face_recognizer", original);

		char key = (char)waitKey(10);
		// Exit this loop on escape:
		if (key == 32)
			break;
	}
}

void FaceSetup::frontalFaceSnapshot(VideoCapture cap, string username) {
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
	classifier.load(lbp_frontalface_file);

	myfile.open(faces_csv_file, ios_base::app);

	// Holds the current frame from the webcam:
	Mat frame;
	while(photosTaken <= NUM_PHOTOS) {
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

			if (!face_resized.empty()){
				string filename = username + to_string(photosTaken);
				imwrite("faces/" + filename + ".jpg", face_resized); // save as jpg in faces folder
				string output = "faces/" + filename + ".jpg;" + to_string(id) + ";" + username;
				putText(original, "TAKING PHOTO " + to_string(photosTaken), Point(220, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 2);
				photosTaken++;

				myfile << output << endl;
			}
		}
		// Show the image from cam
		imshow("face_recognizer", original);
	}
	myfile << endl;
	myfile.close();
}

void FaceSetup::startSetup(string username){

	// Get a handle to the Video device:
	VideoCapture cap(deviceId);
	if (!cap.isOpened()) {
		cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
		return;
	}

	wait(cap);
	brightnessCheck(cap);
	wait(cap);
	frontalFaceSnapshot(cap, username);
}