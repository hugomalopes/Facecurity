#include "FaceBuilder.h"

FaceBuilder::FaceBuilder()
{
}

FaceBuilder::~FaceBuilder()
{
}

string FaceBuilder::getFilename() { return csv_file; }

vector<Mat> FaceBuilder::getImages() { return images; }

vector<int> FaceBuilder::getIdentifiers() { return ids; }

unordered_map<int, string> FaceBuilder::getNamesMap() { return namesMap; }

void FaceBuilder::readCSV() {
	char separator = ';';
	std::ifstream file(getFilename().c_str(), ifstream::in);
	cout << getFilename() << endl;
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, id, name;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, id, separator);
		getline(liness, name);
		if (!path.empty() && !id.empty() && !name.empty()) {
			images.push_back(imread(path, 0));
			ids.push_back(atoi(id.c_str()));
			namesMap.insert({ atoi(id.c_str()), name });
		}
	}
}