/*
* Class: Particle Loader
* Author: Robert Ciborowski
* Date: 02/08/2017
* Description: A class that is made to load particles.
*/

#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#include "../../2D/Texture/TextureLoader.h"

namespace Aela {
	class ParticleLoader : public TextureLoader {
		public:
			ParticleLoader();
			virtual ~ParticleLoader();

			virtual void expose(LuaManager& mgr);

			virtual bool load(std::unordered_map<std::string, Resource*>* resources, std::string src);
	};
}
