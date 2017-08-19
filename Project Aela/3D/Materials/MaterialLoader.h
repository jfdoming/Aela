/*
* Class: Material Loader
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for loading material files.
*/

#pragma once

#include "../../Resource Management/ResourceLoader.h"
#include "../../2D/Texture/TextureLoader.h"

namespace Aela {
	class MaterialLoader : public TextureLoader {
		public:
			MaterialLoader();
			virtual ~MaterialLoader();

			virtual void expose(LuaManager& mgr);
			virtual bool load(ResourceMap& resources, std::string src);

		private:
			unsigned int MATERIAL_PATH_START_POSITION = 7;
			unsigned int MATERIAL_NAME_START_POSITION = 7;
	};
}