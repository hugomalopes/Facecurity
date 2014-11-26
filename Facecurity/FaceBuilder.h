#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;
using namespace std;

class FaceBuilder
{
	// These vectors hold the images and corresponding labels:
	vector<Mat> images;
	vector<int> ids;
	unordered_map<int, string> namesMap;
	const string csv_file = "faces_csv.ext";

	public:
		FaceBuilder();
		~FaceBuilder();
		string getFilename();
		vector<Mat> getImages();
		vector<int> getIdentifiers();
		unordered_map<int, string> getNamesMap();
		void readCSV();
};

