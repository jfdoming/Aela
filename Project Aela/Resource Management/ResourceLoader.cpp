#include "ResourceLoader.h"
#include "../Utilities/strut.h"

std::string Aela::ResourceLoader::getErrorMessage() {
	return errorMessage;
}

bool Aela::ResourceLoader::isValid(std::ifstream& in) {
	return (in.is_open() && in.good() && !in.fail() && !in.bad());
}

std::istream& Aela::ResourceLoader::getline(std::ifstream& in, std::string& line) {
	auto& ret = std::getline(in, line);
	trim(line);
	return ret;
}

int Aela::ResourceLoader::fopen_s(FILE **f, const char *name, const char *mode) {
	int ret = 0;
	assert(f);
	*f = fopen(name, mode);
	if (!*f)
		ret = errno;
	return ret;
}
