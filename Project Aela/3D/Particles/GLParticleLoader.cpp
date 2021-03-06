/*
* Class: Particle Loader
* Author: Robert Ciborowski
* Date: 02/08/2017
* Description: A class that is made to load particles.
*/

#include "GLParticleLoader.h"

using namespace Aela;

GLParticleLoader::GLParticleLoader() {
}

GLParticleLoader::~GLParticleLoader() {
}

void GLParticleLoader::expose(LuaManager& mgr) {
	// only expose part of the class to Lua
	luabridge::getGlobalNamespace(mgr.getLuaState())
		.beginClass<Aela::GLTextureLoader>("ParticleLoader")
		.endClass();

	// expose this object
	// Sorry Waseef, I had to comment this out in order to compile.
	// mgr.exposeObject(this, "textureLoader");
}

bool Aela::GLParticleLoader::load(std::unordered_map<std::string, Resource*>* resources, std::string src) {
	//// This is not done!
	//GLuint modelTextureID;
	//glGenTextures(1, &modelTextureID);

	//// This tells openGL that future functions will reference this texture.
	//glBindTexture(GL_TEXTURE_2D, modelTextureID);

	//unsigned int imageWidth, imageHeight;
	//return loadTexture(src, GL_TEXTURE_2D, &imageWidth, &imageHeight);

	//// (*resources)[src] = res;
	return false;
}