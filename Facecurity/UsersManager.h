#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class UsersManager
{
private:
	const string filename = "users.ext";
	unordered_map<string, string> namePassword;

public:
	UsersManager();
	~UsersManager();
	void readFile();
	void writeFile(string username, string password);
};