#include "ResourceLoader.h"

using namespace Aela;

bool ResourceLoader::isValid(std::ifstream &in) {
	return true;
}

// potentially useful functions for reading from a stream
std::string ResourceLoader::read(std::ifstream &in, int count) {
	std::string val;
	char c;
	int iterations;
	while ((iterations < count) && ((in).get(c))) {
		val = c;
	}
	return val;
}