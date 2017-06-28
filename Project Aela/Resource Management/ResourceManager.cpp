#include "ResourceManager.h"
#include "../Error Handler/ErrorHandler.h"
#include <ios>

using namespace Aela;

bool readMods(std::vector<std::string>& mods) {
	std::ifstream modListFile("mods/mods.list");
	std::string line;

	if (!modListFile) {
		return false;
	}

	while (std::getline(modListFile, line)) {
		mods.emplace_back(line);
	}

	return true;
}

ResourceManager::ResourceManager(int resourceCount) {
	resources.reserve(resourceCount);
}

ResourceManager::~ResourceManager() {
}

// don't call me yet!!!
void ResourceManager::addToLuaInstance(LuaManager& mgr) {
	// only expose part of the class to Lua
	luabridge::getGlobalNamespace(mgr.getLuaState())
		.beginClass<Aela::ResourceManager>("ResourceManager")
		.addFunction("bindGroup", &Aela::ResourceManager::bindGroup)
		.addFunction("bindLoader", &Aela::ResourceManager::bindLoader)
		.addFunction("addToGroup", static_cast<void(Aela::ResourceManager::*)(std::string, bool)>(&Aela::ResourceManager::addToGroup))
		.endClass();

	// expose this object
	mgr.exposeObject(*this, "resourceManager");

	std::vector<std::string> mods;
	if (readMods(mods)) {
		// we must assume we have a list of mods contained in the mod vector
		LuaScript loadingScript;
		loadingScript.initLua(mgr.getLuaState());
		for (auto mod : mods) {
			loadingScript.loadScript(mod + "/res/scripts/resources.lua");
		}
	}
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
