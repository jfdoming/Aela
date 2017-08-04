#include "ResourceMap.h"

using namespace Aela;

ResourceMap::ResourceMap() {
}

ResourceMap::~ResourceMap() {
}

void ResourceMap::reserve(int count) {
	resources.reserve(count);
}

bool ResourceMap::contains(std::string src) {
	auto iter = resources.find(src);
	return iter != resources.end();
}

Resource* ResourceMap::get_impl(std::string key) {
	auto iter = resources.find(key);
	if (iter == resources.end()) {
		return nullptr;
	}

	return iter->second;
}

void ResourceMap::put(std::string src, Resource* value) {
	resources[src] = value;
}
