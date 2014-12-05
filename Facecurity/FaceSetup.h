#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;
using namespace std;

class FaceSetup
{
	private:
		const string WINDOW_NAME;
		const string FACES_CSV_FILE;
		const string HAAR_FRONTALFACE_FILE;
		const string HAAR_PROFILEFACE_FILE;
		const string LBP_FRONTALFACE_FILE;
		const string LBP_PROFILEFACE_FILE;
		const string HAAR_SMILE_FILE;
		const int DEVICEID;
		// Used to reshape the faces this size
		const int IM_WIDTH;
		const int IM_HEIGHT;
		const double MIN_BRIGHTNESS;
		const int NUM_PHOTOS;
		string username;
		int id;
		void getBrightness(Mat& frame, double& brightness);
		void wait(VideoCapture cap, string msg);
		void brightnessCheck(VideoCapture cap);
		void frontalFacePrepare(VideoCapture cap);
		void frontalFaceSnapshot(VideoCapture cap);
		void profileFacePrepare(VideoCapture cap);
		void smilePrepare(VideoCapture cap);
		void smileSnapshot(VideoCapture cap);

	public:
		FaceSetup(string uname);
		~FaceSetup();
		void startSetup();
};

