#include "stdafx.h"
#include "ResourceManager.h"

using namespace Aela;

const string ResourceManager::TEXT_LOAD_PATH = "res/text";
const string ResourceManager::IMAGE_LOAD_PATH = "res/images";

ResourceManager::ResourceManager(int resourceCount) {
	resources.reserve(resourceCount);
}

ResourceManager::~ResourceManager() {
}

void ResourceManager::loadText(string src) {
	TextResource res(TEXT_LOAD_PATH + src);
	resources.emplace(src, res);
}

Resource ResourceManager::obtain(string src) {
	return resources.at(src);
}