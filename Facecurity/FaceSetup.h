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
	const string faces_csv_file;
	const string haar_profileface_file;
	const string lbp_frontalface_file;
	const string lbp_profileface_file;
	const string haar_smile_file;
	const int deviceId;
	// Used to reshape the faces this size
	const int im_width;
	const int im_height;

	const int NUM_PHOTOS;

public:
	FaceSetup();
	~FaceSetup();
	void getBrightness(Mat& frame, double& brightness);
	void wait(VideoCapture cap);
	void brightnessCheck (VideoCapture cap);
	void frontalFaceSnapshot(VideoCapture cap, string username);
	void startSetup(string username);
};

