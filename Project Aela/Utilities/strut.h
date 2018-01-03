/*
* Name: String Utilities
* Author: Julian Dominguez-Schatz
* Date: August 2017
* Description: A file used for performing operations on strings.
*/

#pragma once

#include <string>
#include <algorithm>
#include <cctype>

// Note: If I had C++ 2017, I woudn't need to include this. I could also just make my own function for clamping.
#include <glm/glm.hpp>

// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

static inline std::string toStringWithDecimal(float value, int places) {
	std::string s = std::to_string(value);
	s = s.substr(0, glm::clamp((int) s.find('.') + places + 1, 0, (int) s.size()));
	return s;
}

static std::string toStringWithATrailingZero(float value) {
	std::string s = std::to_string(value);
	bool trailing = false;
	int trailingPos = 0;
	for (unsigned int pos = 0; pos < s.length(); pos++) {
		if (s.at(pos) == '0' && !trailing) {
			trailing = true;
			trailingPos = pos;
		} else if (s.at(pos) != '0') {
			trailing = false;
		}
	}
	if (trailing) {
		s = s.substr(0, trailingPos + 1);
	}
	return s;
}

static inline bool startsWith(std::string& haystack, std::string& needle) {
	return needle.length() <= haystack.length()
		&& equal(needle.begin(), needle.end(), haystack.begin());
}
