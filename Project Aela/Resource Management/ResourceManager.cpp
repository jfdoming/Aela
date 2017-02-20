#include "ResourceManager.h"

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
	std::ifstream in;
	in.open(src);

	Resource* res = loader->load(in);
	in.close();

	//std::cout << &res << std::endl;
	resources.emplace(res->src, res);
	//std::cout << "meme: \"" << (NULL == &resources.at(src)) << "\"" << std::endl;
	return true;
}

template <class T> T& ResourceManager::obtain(std::string src) {
	//std::cout << "meme: \"" << (NULL == &(resources.find(src))) << "\"" << std::endl;
	T* ref = resources.at(src);
	return *ref;
}

std::string Aela::ResourceManager::getNewCrucialInvalidResourceKey() {
	return crucialInvalidResourceKey;
}

std::vector<std::string>& Aela::ResourceManager::getNewInvalidResourceKeys() {
	return invalidResourceKeys;
}
