#include "ResourceManager.h"
#include "../Error Handler/ErrorHandler.h"
#include <ios>

using namespace Aela;

ResourceManager::ResourceManager(int resourceCount) {
	resources.reserve(resourceCount);
}

ResourceManager::~ResourceManager() {
}

// don't call me yet!!!
void ResourceManager::expose(LuaManager& mgr) {
	// only expose part of the class to Lua
	luabridge::getGlobalNamespace(mgr.getLuaState())
		.beginClass<Aela::ResourceManager>("ResourceManager")
		.addFunction("bindGroup", &Aela::ResourceManager::bindGroup)
		.addFunction("bindLoader", &Aela::ResourceManager::bindLoader)
		.addFunction("addToGroup", static_cast<void(Aela::ResourceManager::*)(std::string, bool)>(&Aela::ResourceManager::addToGroup))
		.endClass();

	// expose this object
	mgr.exposeObject(this, "resourceManager");
}

void ResourceManager::bindLoader(ResourceLoader* loader) {
	this->boundLoader = loader;
}

void ResourceManager::bindGroup(std::string group) {
	// ensure a group exists to add to
	auto iter = groups.find(group);
	if (iter == groups.end()) {
		groups.emplace(group, ResourceGroup(group));
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
	auto group = &(iter->second);

	// load all resources in the group
	Status returnStatus = Status::OK;

	for (ResourceQuery query : *(group->getQueries())) {
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
	auto group = &(iter->second);

	// unload all resources in the group
	for (ResourceQuery query : *(group->getQueries())) {
		// unload the resource
		unload(query.getSrc());
	}
	return Status::OK;
}

void ResourceManager::addToGroup(std::string src, bool crucial) {
	ResourceQuery query(src, crucial, boundLoader);
	addToGroup(query);
}

void ResourceManager::addToGroup(ResourceQuery& query) {
	boundGroup->getQueries()->push_back(query);
}

ResourceManager::Status ResourceManager::load(std::string src, bool crucial, ResourceLoader& loader) {
	ResourceQuery query(src, crucial, &loader);
	return load(query);
}

ResourceManager::Status ResourceManager::load(ResourceQuery& query) {
	bool crucial = query.isCrucial();
	std::string src = query.getSrc();

	bool success = boundLoader->load(resources, src);
	
	if (!success) {
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
	Resource* res;

	if (resources.get(src, res) && res != nullptr) {
		delete res;
	}
}

std::string Aela::ResourceManager::getNewCrucialInvalidResourceKey() {
	return crucialInvalidResourceKey;
}

std::vector<std::string>& Aela::ResourceManager::getNewInvalidResourceKeys() {
	return invalidResourceKeys;
}
