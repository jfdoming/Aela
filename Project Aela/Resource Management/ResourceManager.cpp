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
	this->boundLoader = loader;
}

void ResourceManager::bindGroup(std::string group) {
	// ensure a group exists to add to
	auto iter = groups.find(group);
	if (groups.find(group) == groups.end()) {
		groups.emplace(group, std::vector<ResourceQuery>());
		this->boundGroup = &(groups.find(group)->second);
	} else {
		this->boundGroup = &(iter->second);
	}
}

ResourceManager::Status ResourceManager::loadGroup(std::string name) {
	// ensure a group exists to load
	auto iter = groups.find(name);
	if (groups.find(name) == groups.end()) {
		return Status::FAILED;
	}

	// get the group to load
	auto group = iter->second;

	// load all resources in the group
	Status returnStatus = Status::OK;
	for (ResourceQuery query : group) {
		// make sure we are using the correct resource loader
		ResourceLoader* resourceLoader = query.getLoader();
		if (boundLoader != resourceLoader) {
			bindLoader(resourceLoader);
		}

		// load the resource
		Status status = load(query);

		// interpret the result
		if (status == Status::ABORT) {
			return status;
		}
		if (status == Status::FAILED) {
			returnStatus = status;
		}
	}
	return returnStatus;
}

ResourceManager::Status ResourceManager::unloadGroup(std::string name) {
	// ensure a group exists to unload
	auto iter = groups.find(name);
	if (groups.find(name) == groups.end()) {
		return Status::FAILED;
	}

	// get the group to unload
	auto group = iter->second;

	// unload all resources in the group
	for (ResourceQuery query : group) {
		// load the resource
		unload(query.getSrc());
	}
	return Status::OK;
}

void ResourceManager::addToGroup(std::string src, bool crucial) {
	ResourceQuery query(src, crucial, boundLoader);
	addToGroup(query);
}

void ResourceManager::addToGroup(ResourceQuery& query) {
	boundGroup->push_back(query);
}

ResourceManager::Status ResourceManager::load(std::string src, bool crucial, ResourceLoader& loader) {
	ResourceQuery query(src, crucial, &loader);
	return load(query);
}

ResourceManager::Status ResourceManager::load(ResourceQuery& query) {
	bool valid = true;
	bool crucial = query.isCrucial();
	std::string src = query.getSrc();

	std::ifstream in;
	in.flags(std::ios::binary | std::ios::in);
	in.open(src);

	if (!in.is_open() || in.fail()) {
		// for now, errors are automatically handled here (may change after consultation)
		AelaErrorHandling::consoleWindowError("Resource Manager", "Failed to open file \"" + src + "\" for reading!");
		valid = false;
	}

	if (valid && boundLoader->isValid(in)) {
		Resource* res = boundLoader->load(in);
		
		if (res == NULL) {
			valid = false;
		} else {
			resources.emplace(src, res);
		}
	}

	in.close();
	
	if (!valid) {
		// cannot load the resource
		if (crucial) {
			crucialInvalidResourceKey = src;
			return Status::ABORT;
		} else {
			invalidResourceKeys.push_back(src);
			return Status::FAILED;
		}
	}

	return Status::OK;
}

void ResourceManager::unload(std::string src) {
	delete obtain_impl(src);
}

Resource* ResourceManager::obtain_impl(std::string src) {
	auto iter = resources.find(src);
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
