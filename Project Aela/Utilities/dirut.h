/*
* Name: Directory Utilities
* Author: Robert Ciborowski
* Date: 18/02/2019
* Description: Please come up with a better name for this file.
* 
*              Also, we should have a version of this file for non-Windows
*              operating systems. Or maybe we could just replace the contents
*              of this function with those necessary for those other systems.
*/

#pragma once

#include <windows.h>
#include <string>

// https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
// I think this is broken.
/*static bool directoryExists(const std::string& dirName_in) {
	std::cout << "dirExists\n";
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES) {
		// Something is wrong with your path!
		return false;
	}

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
		// This is a directory!
		std::cout << "The directory exists!\n";
		return true;
	}

	// This is not even a directory!
	return false;
}*/

static bool makeDirectory(const std::string& directory) {
    return CreateDirectory(directory.c_str(), NULL);
}