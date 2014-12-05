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

class FaceLogin
{
	private:
		const string LBP_FRONTALFACE_FILE;
		const int DEVICEID;
		unordered_map<int, string> namesMap;
		Ptr<FaceRecognizer> model;
		int im_width;
		int im_height;

	public:
		FaceLogin();
		~FaceLogin();
		void startRecognition();
};