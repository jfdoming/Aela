#include "ResourceManager.h"

using namespace Aela;

ResourceManager::ResourceManager(int resourceCount) {
	resources.reserve(resourceCount);
}

ResourceManager::~ResourceManager() {
}

bool ResourceManager::loadTexture(std::string src, bool crucial) {
	TextureResource * res = new TextureResource(src);
	//std::cout << &res << std::endl;
	resources.emplace(res->src, res);
	//std::cout << "meme: \"" << (NULL == &resources.at(src)) << "\"" << std::endl;
	return true;
}

Resource& ResourceManager::obtain(std::string src) {
	//std::cout << "meme: \"" << (NULL == &(resources.find(src))) << "\"" << std::endl;
	Resource * ref = resources.at(src);
	return *ref;
}

std::string Aela::ResourceManager::getNewCrucialInvalidResourceKey() {
	return crucialInvalidResourceKey;
}

std::vector<std::string>& Aela::ResourceManager::getNewInvalidResourceKeys() {
	return invalidResourceKeys;
}
