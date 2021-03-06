#include "FaceSetup.h"
#include "FaceLoader.h"

FaceSetup::FaceSetup(string uname) : WINDOW_NAME("Facecurity - Setup"),
						FACES_CSV_FILE("faces_csv.ext"),
						HAAR_FRONTALFACE_FILE("haarcascade_frontalface_default.xml"),
						HAAR_PROFILEFACE_FILE("haarcascade_profileface.xml"),
						HAAR_SMILE_FILE("haarcascade_smile.xml"),
						LBP_FRONTALFACE_FILE("lbpcascade_frontalface.xml"),
						LBP_PROFILEFACE_FILE("lbpcascade_profileface.xml"),
						DEVICEID(0),
						IM_WIDTH(240),
						IM_HEIGHT(240),
						MIN_BRIGHTNESS(0.95),
						NUM_PHOTOS(20) { 
	username = uname;

	FaceLoader *faceLoader = new FaceLoader();
	try {
		id = faceLoader->readCSV() + 1;
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << "file" << "\". Reason: " << e.msg << endl; //mudar depois
		exit(1);
	}

	unordered_map<int, string> namesMap = faceLoader->getNamesMap();
	for(auto entry : namesMap) {
		if(username == entry.second) {
			cerr << "The user " << username << " is already registered." << endl;
			exit(1);
		}
	}

	delete(faceLoader);
}

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

void FaceSetup::wait(VideoCapture cap, string msg) {
	// Holds the current frame from the webcam:
	Mat frame;
	for (;;) {
		cap >> frame;
		// Clone the current frame:
		Mat original = frame.clone();

		// Put the message in the frame
		putText(original, msg, Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 1);

		// Show the image from cam
		imshow(WINDOW_NAME, original);

		char key = (char)waitKey(5);
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

		if(brightness > MIN_BRIGHTNESS) {
			putText(original, "LUMINOSITY OK. PRESS SPACE TO CONTINUE.", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 1);
			// Show the image from cam
			imshow(WINDOW_NAME, original);

			char key = (char)waitKey(5);
			// Exit this loop on escape:
			if (key == 32)
				break;
		}
		else {
			putText(original, "SWITCH ON THE LIGHTS OR GO TO A BRIGHTER PLACE.", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(175, 0, 0), 1);
			// Show the image from cam
			imshow(WINDOW_NAME, original);
			char key = (char)waitKey(5);
		}		
	}
}

void FaceSetup::frontalFacePrepare(VideoCapture cap) {
	// Create a Cascade Classifier using the xml file specifed.
	CascadeClassifier classifier;
	classifier.load(LBP_FRONTALFACE_FILE );

	// Holds the current frame from the webcam:
	Mat frame;
	for(;;) {
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
		if (faces.size() > 1) {
			putText(original, "ONE PERSON AT A TIME, PLEASE", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(175, 0, 0), 1);
			// Show the image from cam
			imshow(WINDOW_NAME, original);
			char key = (char)waitKey(5);
		}
		else if (faces.size() == 1){
			// Crop the face from the image
			face = gray(faces[0]);
			rectangle(original, faces[0], CV_RGB(255, 255, 255), 1);

			putText(original, "FACE DETECTED. KEEP IT STABILIZED AND PRESS SPACE.", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 1);
			// Show the image from cam
			imshow(WINDOW_NAME, original);
			// Only allows to proceed if it is detecting a face
			char key = (char)waitKey(5);
			// Exit this loop on escape:
			if (key == 32)
				break;
		}
		else {
			putText(original, "STABILIZE YOUR FACE AT THE CENTER.", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(175, 0, 0), 1);
			imshow(WINDOW_NAME, original);
			char key = (char)waitKey(5);
		}	
	}
}

void FaceSetup::frontalFaceSnapshot(VideoCapture cap) {
	int photosTaken = 1;
	bool takePhoto = false;
	ofstream myfile;
	
	// Create a Cascade Classifier using the xml file specifed.
	CascadeClassifier classifier;
	classifier.load(LBP_FRONTALFACE_FILE);

	myfile.open(FACES_CSV_FILE, ios_base::app);

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
			putText(original, "ONE PERSON AT A TIME, PLEASE", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(175, 0, 0), 1);
		else if (faces.size() == 1){
			// Crop the face from the image
			face = gray(faces[0]);
			// Resizing the face is necessary for Fisherfaces algotithm
			cv::resize(face, face_resized, Size(IM_WIDTH, IM_HEIGHT), 1.0, 1.0, INTER_CUBIC);
			rectangle(original, faces[0], CV_RGB(255, 255, 255), 1);

			if (!face_resized.empty()){
				string filename = username + to_string(photosTaken);
				imwrite("faces/" + filename + "_frontalface.jpg", face_resized); // save as jpg in faces folder
				string output = "faces/" + filename + "_frontalface.jpg;" + to_string(id) + ";" + username + "_frontalface";
				putText(original, "MEMORIZING YOUR FACE: " + to_string(photosTaken*100/NUM_PHOTOS) + "%", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 1);
				photosTaken++;

				myfile << output << endl;

				// Show the image from cam
				imshow(WINDOW_NAME, original);
				char key = (char)waitKey(5);
			}
		}
		else {
			putText(original, "HOLD ON PLEASE.", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(175, 0, 0), 1);
			// Show the image from cam
			imshow(WINDOW_NAME, original);
			char key = (char)waitKey(5);
		}
	}
	myfile << endl;
	myfile.close();
	id++;
}

void FaceSetup::smilePrepare(VideoCapture cap) {
	// Create a Cascade Classifier using the xml file specifed.
	CascadeClassifier classifier;
	classifier.load(LBP_FRONTALFACE_FILE );

	// Holds the current frame from the webcam:
	Mat frame;
	for(;;) {
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
		if (faces.size() > 1) {
			putText(original, "ONE PERSON AT A TIME, PLEASE", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(175, 0, 0), 1);
			// Show the image from cam
			imshow(WINDOW_NAME, original);
			char key = (char)waitKey(5);
		}
		else if (faces.size() == 1) {
			// Crop the face from the image
			face = gray(faces[0]);
			
			CascadeClassifier smileClassifier;
			smileClassifier.load(HAAR_SMILE_FILE);
			vector<Rect_<int>> smiles;
			smileClassifier.detectMultiScale(face, smiles);

			if(smiles.size() > 0) {
				// Searches for the smile detected in the bottom of the face, because the eyes and the nose sometimes are recognized but are on top
				int maxWidth = smiles[0].width;
				for(int i=1; i<smiles.size(); i++) {
					maxWidth = max(smiles[i].width, maxWidth);
				}
				Rect_<int>* trueSmile = NULL;
				for(int i=0; i<smiles.size(); i++) {
					if((smiles[i].width == maxWidth) && (smiles[i].y > (face.rows/2)))
						trueSmile = &smiles[i];
				}
				if(trueSmile != NULL) {
					// Sum the relative position of the smile in the face to the position of the face to draw in the original image
					Rect_<int> smileRect = Rect(faces[0].x + trueSmile->x, faces[0].y + trueSmile->y, trueSmile->width, trueSmile->height);
					rectangle(original, smileRect, CV_RGB(255, 255, 255), 1);
				}
			}
			putText(original, "SMILE DETECTED. PRESS SPACE.", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 1);
			// Show the image from cam
			imshow(WINDOW_NAME, original);
			// Only allows to proceed if it is detecting a face
			char key = (char)waitKey(5);
			// Exit this loop on escape:
			if (key == 32)
				break;
		}
		else {
			putText(original, "STAY FUNNY AND SMILE.", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(175, 0, 0), 1);
			imshow(WINDOW_NAME, original);
			char key = (char)waitKey(5);
		}
	}
}

void FaceSetup::smileSnapshot(VideoCapture cap) {
	int photosTaken = 1;
	bool takePhoto = false;
	ofstream myfile;

	// Create a Cascade Classifier using the xml file specifed.
	CascadeClassifier classifier;
	classifier.load(LBP_FRONTALFACE_FILE );

	myfile.open(FACES_CSV_FILE, ios_base::app);

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
		Mat face, face_resized;;
		if (faces.size() > 1) {
			putText(original, "ONE PERSON AT A TIME, PLEASE", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(175, 0, 0), 1);
			// Show the image from cam
			imshow(WINDOW_NAME, original);
			char key = (char)waitKey(5);
		}
		else if (faces.size() == 1) {
			// Crop the face from the image
			face = gray(faces[0]);
			
			CascadeClassifier smileClassifier;
			smileClassifier.load(HAAR_SMILE_FILE);
			vector<Rect_<int>> smiles;
			smileClassifier.detectMultiScale(face, smiles);

			if(smiles.size() > 0) {
				// Searches for the smile detected in the bottom of the face, because the eyes and the nose sometimes are recognized but are on top
				int maxWidth = smiles[0].width;
				for(int i=1; i<smiles.size(); i++) {
					maxWidth = max(smiles[i].width, maxWidth);
				}
				Rect_<int>* trueSmile = NULL;
				for(int i=0; i<smiles.size(); i++) {
					if((smiles[i].width == maxWidth) && (smiles[i].y > (face.rows/2)))
						trueSmile = &smiles[i];
				}
				if(trueSmile != NULL) {
					// Sum the relative position of the smile in the face to the position of the face to draw in the original image
					Rect_<int> smileRect = Rect(faces[0].x + trueSmile->x, faces[0].y + trueSmile->y, trueSmile->width, trueSmile->height);
					rectangle(original, smileRect, CV_RGB(255, 255, 255), 1);

					// Resizing the face is necessary for Fisherfaces algotithm
					cv::resize(face, face_resized, Size(IM_WIDTH, IM_HEIGHT), 1.0, 1.0, INTER_CUBIC);
					if (!face_resized.empty()){
						string filename = username + to_string(photosTaken);
						imwrite("faces/" + filename + "_smile.jpg", face_resized); // save as jpg in faces folder
						string output = "faces/" + filename + "_smile.jpg;" + to_string(id) + ";" + username + "_smile";
						putText(original, "MEMORIZING YOUR SMILE: " + to_string(photosTaken*100/NUM_PHOTOS) + "%", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100, 255, 0), 1);
						photosTaken++;

						myfile << output << endl;

						// Show the image from cam
						imshow(WINDOW_NAME, original);
						char key = (char)waitKey(5);
					}
				}
			}
			else {
				putText(original, "SMILE.", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(175, 0, 0), 1);
				// Show the image from cam
				imshow(WINDOW_NAME, original);
				char key = (char)waitKey(5);
			}

		}
		else {
			putText(original, "SMILE.", Point(20, 30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(175, 0, 0), 1);
			imshow(WINDOW_NAME, original);
			char key = (char)waitKey(5);
		}
	}
	myfile << endl;
	myfile.close();
	id++;
}

void FaceSetup::startSetup(){

	// Get a handle to the Video device:
	VideoCapture cap(DEVICEID);
	if (!cap.isOpened()) {
		cerr << "Capture Device ID " << DEVICEID << "cannot be opened." << endl;
		return;
	}

	wait(cap, "FOLLOW THE TIPS GIVEN IN EACH STEP. PRESS SPACE TO START.");
	brightnessCheck(cap);
	frontalFacePrepare(cap);
	frontalFaceSnapshot(cap);
	smilePrepare(cap);
	smileSnapshot(cap);
}