/*
* Class: Material Loader
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for loading material files.
*/

#pragma once

#include "ResourceLoader.h"

namespace Aela {
	class MaterialLoader : public ResourceLoader {
		public:
			MaterialLoader();
			virtual ~MaterialLoader();

			virtual void expose(LuaManager& mgr);
			virtual bool load(std::unordered_map<std::string, Resource*>* resources, std::string src);
		private:

			GLuint loadDDSToGLuint(std::string filePath);
	};
}