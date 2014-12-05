#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <unordered_map>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;
using namespace std;

class FaceLogin
{
	private:
		const string WINDOW_NAME;
		const string LBP_FRONTALFACE_FILE;
		const int DEVICEID;
		const double CONFIDENCETHRESHOLD;
		const int FACERECOGNITIONTIME;
		unordered_map<int, string> namesMap;
		Ptr<FaceRecognizer> model;
		int im_width;
		int im_height;
		void recognizeFrontalFace(VideoCapture cap);

	public:
		FaceLogin();
		~FaceLogin();
		void startRecognition();
};