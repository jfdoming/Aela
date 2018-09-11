/*
* Class: Tile Loader
* Author: Robert Ciborowski
* Date: 27/12/2017
* Description: A class for cloning a template tile model and creating a new model for a tile.
*              Only one instance of this class should be used, and at the beginning of the
*              program runtime. I must do this since I need to clone the template tile (rather
*              than having one model per tile) (the resource management system does not allow
*              for resource cloning).
*              Note: if you want to access these tile models later via the resource manager, ask
*              for a resource with an src of "tileX" (where X is the tile #).
*/

#pragma once

#include "../../Project Aela/3D/Models/Model.h"
#include "../../Project Aela/Resource Management/ResourceLoader.h"
#include "../../Project Aela/Resource Management/ResourceMap.h"
#include "../../Project Aela/Resource Management/ResourceManager.h"
#include <map>

using namespace Aela;

namespace Game {
	class TileLoader : public ResourceLoader {
		public:
			TileLoader() {}
			virtual ~TileLoader() {}

			// src represents the source of the template model to use.
			virtual bool load(ResourceMap& resources, std::string src);
			void setResourceManager(ResourceManager* resourceManager);
			void useMaterial(Material* material);
			unsigned int getTilesLoaded();

		private:
			ResourceManager* resourceManager;
			Material* materialToUse;
			unsigned int tilesLoaded = 0;
	};
}