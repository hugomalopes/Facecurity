#include "FaceLoader.h"

FaceLoader::FaceLoader()
{
}

FaceLoader::~FaceLoader()
{
}

string FaceLoader::getFilename() { return csv_file; }

vector<Mat> FaceLoader::getImages() { return images; }

vector<int> FaceLoader::getIdentifiers() { return ids; }

unordered_map<int, string> FaceLoader::getNamesMap() { return namesMap; }

void FaceLoader::readCSV() {
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