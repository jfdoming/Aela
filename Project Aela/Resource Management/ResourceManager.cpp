#include "ResourceManager.h"
#include "../Error Handler/ErrorHandler.h"
#include <ios>

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
	in.flags(std::ios::binary | std::ios::in);
	in.open(src);

	if (!in.is_open() || in.fail()) {
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

void ResourceManager::unload(std::string src) {
	delete obtain_impl(src);
}

Resource* ResourceManager::obtain_impl(std::string src) {
	std::unordered_map<std::string, Resource*>::const_iterator iter = resources.find(src);
	if (iter == resources.end()) {
		return NULL;
	}
	return iter->second;
}

std::string Aela::ResourceManager::getNewCrucialInvalidResourceKey() {
	return crucialInvalidResourceKey;
}

std::vector<std::string>& Aela::ResourceManager::getNewInvalidResourceKeys() {
	return invalidResourceKeys;
}
