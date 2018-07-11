#include "GenericMaterialLoader.h"
#include "../../Project Aela/3D/Materials/Material.h"

Aela::GenericMaterialLoader::GenericMaterialLoader() {}

Aela::GenericMaterialLoader::~GenericMaterialLoader() {}

void Aela::GenericMaterialLoader::expose(LuaManager & mgr) {
	// only expose part of the class to Lua
	luabridge::getGlobalNamespace(mgr.getLuaState())
		.beginClass<Aela::GenericMaterialLoader>("GenericMaterialLoader")
		.endClass();

	// Sorry Waseef, Julian had to comment this out in order to compile.
	// mgr.exposeObject(this, "textureLoader");
}

bool Aela::GenericMaterialLoader::load(ResourceMap& resources, std::string src) {
	Material* material = new Material(src);
	if (defaultTexture != nullptr) {
		material->setTexture(defaultTexture);
	}
	resources.put(src, material);
	return true;
}

void Aela::GenericMaterialLoader::setDefaultTexture(Texture* defaultTexture) {
	this->defaultTexture = defaultTexture;
}
