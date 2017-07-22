#include "SkyboxLoader.h"
#include "Skybox.h"

using namespace Aela;


const std::string SkyboxLoader::paths[] = {
	"/right.dds",
	"/left.dds",
	"/up.dds",
	"/down.dds",
	"/back.dds",
	"/front.dds"
};

SkyboxLoader::SkyboxLoader() {
}

SkyboxLoader::~SkyboxLoader() {
}

void SkyboxLoader::expose(LuaManager& mgr) {
	// only expose part of the class to Lua
	luabridge::getGlobalNamespace(mgr.getLuaState())
		.beginClass<Aela::SkyboxLoader>("SkyboxLoader")
		.endClass();

	// expose this object
	// Sorry Waseef, I had to comment this out in order to compile.
	// mgr.exposeObject(this, "textureLoader");
}

Resource* Aela::SkyboxLoader::load(std::string src) {
	Skybox* res = new Skybox();

	GLuint* cubeMapTexture = res->getTexture();
	glGenTextures(1, cubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMapTexture);

	for (unsigned int i = 0; i < 6; i++) {
		std::ifstream in;
		if (!open(in, src)) {
			return nullptr;
		}

		loadTexture(in, cubeMapTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

		in.close();
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return res;
}
