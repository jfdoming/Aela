#include "ResourceLoader.h"

std::string Aela::ResourceLoader::getErrorMessage() {
	return errorMessage;
}

bool Aela::ResourceLoader::open(std::ifstream& in, std::string src) {
	in.flags(std::ios::binary | std::ios::in);
	in.open(src);
	return (in.is_open() && !in.fail());
}