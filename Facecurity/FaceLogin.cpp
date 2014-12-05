#include "FaceLogin.h"
#include "FaceLoader.h"

FaceLogin::FaceLogin() : WINDOW_NAME("Facecurity - Login"),
						 LBP_FRONTALFACE_FILE("lbpcascade_frontalface.xml"),
						 DEVICEID(0),
						 CONFIDENCETHRESHOLD(6000.0),
						 FACERECOGNITIONTIME(20) {
	FaceLoader *faceLoader = new FaceLoader();
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
	im_width = faceLoader->getImages()[0].cols;
	im_height = faceLoader->getImages()[0].rows;
	
	// Create a FaceRecognizer. If it's 0 in 1st argument train it with all given images. 2nd argument is the confidence threshold value for acceptance.
	model = createFisherFaceRecognizer(0, CONFIDENCETHRESHOLD);
	model->train(faceLoader->getImages(), faceLoader->getIdentifiers());
	
	delete faceLoader;
}

FaceLogin::~FaceLogin() {}

void FaceLogin::recognizeFrontalFace(VideoCapture cap) {

	int totalFrames = 0;
	int unknownMatches = 0;
	unordered_map<int, int> personMatchesMap;
	unordered_map<int, int> frontalfaceMatchesMap;
	unordered_map<int, int> smileMatchesMap;
	// Create a Cascade Classifier using the xml file specifed.
	CascadeClassifier classifier;
	classifier.load(LBP_FRONTALFACE_FILE);

	time_t initialTime = time(0);
	// Holds the current frame from the webcam:
	Mat frame;
	// Run during 
	while(difftime(time(0), initialTime) < FACERECOGNITIONTIME) {
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
		if(faces.size() == 1){
			// Crop the face from the image
			face = gray(faces[0]);
			// Resizing the face is necessary for Fisherfaces algotithm
			Mat face_resized;
			cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
			// Now perform the prediction
			int label_predicted;
			double confidence_predicted;
			model->predict(face_resized, label_predicted, confidence_predicted);

			if(label_predicted == -1)
				unknownMatches++;
			else {
				if((label_predicted % 2) == 0) {
					personMatchesMap[label_predicted] = personMatchesMap[label_predicted] + 1;
					frontalfaceMatchesMap[label_predicted] = frontalfaceMatchesMap[label_predicted] + 1;
				}
				else {
					personMatchesMap[label_predicted -1] = personMatchesMap[label_predicted -1] + 1;
					smileMatchesMap[label_predicted - 1] = smileMatchesMap[label_predicted - 1] + 1;
				}
			}
		}
		totalFrames++;
		// Show the result:
		imshow(WINDOW_NAME, original);
		char key = (char)waitKey(5);
	}
}

void FaceLogin::startRecognition() {
	
	// Get a handle to the Video device:
	VideoCapture cap(DEVICEID);
	if (!cap.isOpened()) {
		cerr << "Capture Device ID " << DEVICEID << "cannot be opened." << endl;
		return;
	}

	recognizeFrontalFace(cap);
}