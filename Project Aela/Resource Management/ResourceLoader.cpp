#include "ResourceLoader.h"

std::string Aela::ResourceLoader::getErrorMessage() {
	return errorMessage;
}

bool Aela::ResourceLoader::isValid(std::ifstream& in) {
	return (in.is_open() && in.good() && !in.fail() && !in.bad());
}