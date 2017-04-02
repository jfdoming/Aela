#include "ResourceManager.h"
#include "../ErrorHandler.h"

using namespace Aela;

ResourceManager::ResourceManager(int resourceCount) {
	resources.reserve(resourceCount);
}

ResourceManager::~ResourceManager() {
}

void ResourceManager::bindLoader(ResourceLoader* loader) {
	this->loader = loader;
}

bool ResourceManager::load(std::string src, bool crucial) {
	bool valid = true;
	std::ifstream in;
	in.open(src);

	if (!in.is_open()) {
		// for now, errors are automatically handled here (may change after consultation)
		AelaErrorHandling::consoleWindowError("Resource Manager", "Failed to open file \"" + src + "\" for reading!");
		valid = false;
	}

	if (valid && loader->isValid(in)) {
		Resource* res = loader->load(in);
		
		if (res == NULL) {
			valid = false;
		} else {
			resources.emplace(src, res);
		}
	}
	
	if (!valid) {
		// cannot load the resource
		if (crucial) {
			crucialInvalidResourceKey = src;
		} else {
			invalidResourceKeys.push_back(src);
		}
	}

	in.close();
	return valid;
}

template <class T> T* ResourceManager::obtain(std::string src) {
	std::iterator iter = resources.find(src);
	if (iter == resources.end()) {
		return NULL;
	}
	return iter.second;
}

std::string Aela::ResourceManager::getNewCrucialInvalidResourceKey() {
	return crucialInvalidResourceKey;
}

std::vector<std::string>& Aela::ResourceManager::getNewInvalidResourceKeys() {
	return invalidResourceKeys;
}