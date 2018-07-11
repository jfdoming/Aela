/*
* Class: OpenGL Generic Material Loader
* Author: Robert Ciborowski
* Date: 4/28/2018
* Description: A class which creates a new, blank material with a
*              resource ID of the given src. This might be moved
*              into the Aela engine itself.
*/

#pragma once
#include "../../Project Aela/Resource Management/ResourceLoader.h"
#include "../../Project Aela/2D/Texture/Texture.h"

namespace Aela {
	class 
		GenericMaterialLoader : public ResourceLoader {
		public:
			GenericMaterialLoader();
			virtual ~GenericMaterialLoader();

			virtual void expose(LuaManager& mgr);
			virtual bool load(ResourceMap& resources, std::string src);

			void setDefaultTexture(Texture* defaultTexture);

		private:
			// const std::string defaultPathForMaterials = "res/materials";

			Texture* defaultTexture = nullptr;
	};
}