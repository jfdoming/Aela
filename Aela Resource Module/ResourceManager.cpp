#include "ResourceManager.h"

using namespace Aela;

ResourceManager::ResourceManager(int resourceCount) {
	resources.reserve(resourceCount);
}

ResourceManager::~ResourceManager() {
}

bool ResourceManager::loadText(const char * src, bool crucial) {
	//TextResource * res = new TextResource(src);
	//std::cout << &res << std::endl;
	resources.emplace(2, 1);
	//std::cout << "meme: \"" << (NULL == &resources.at(src)) << "\"" << std::endl;
	return true;
}

Resource& ResourceManager::obtain(const char * src) {
	//resources.at(src);
	//std::cout << "meme: \"" << (NULL == &(resources.find(src))) << "\"" << std::endl;
	int ref = resources.at(2);
	return Resource(""); // crashes, has to do with var res above being a local variable? tinker
}

//const char * Aela::ResourceManager::getNewCrucialInvalidResourceKey() {
//	return crucialInvalidResourceKey;
//}

std::vector<const char *>& Aela::ResourceManager::getNewInvalidResourceKeys() {
	return invalidResourceKeys;
}
