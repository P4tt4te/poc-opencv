#include <iostream>
#include <windows.h>
#include <stdio.h>

static std::string getFullPath(const char* _sPath) {
	std::string sFullPathName = "";
	char fullFilename[MAX_PATH];
	GetFullPathName(_sPath, MAX_PATH, fullFilename, nullptr);
	sFullPathName = fullFilename;
	return sFullPathName;
};