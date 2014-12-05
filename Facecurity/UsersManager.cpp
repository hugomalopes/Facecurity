#include "UsersManager.h"

UsersManager::UsersManager() {

}

void UsersManager::readFile() {
	std::ifstream file(filename, ifstream::in);
	if (!file) {
		cerr << "No valid input file was given, please check the given filename." << endl;
	}
	string line, username, password;
	char separator = ';';
	while (getline(file, line)) {
			stringstream liness(line);
			getline(liness, username, separator);
			getline(liness, password);
		if (!username.empty() && !password.empty()) {
			namePassword.insert({username, password});
		}
	}
}

void UsersManager::writeFile(string username, string password) {
  ofstream file;
  file.open(filename, ios_base::app);
  file << username << ";" << password << endl;
  file.close();
}
