/*
* Class: Tile
* Author: Robert Ciborowski
* Date: 03/12/2017
* Description: A class used to represent an in-game tile/space.
*/

#pragma once
#include <string>
#include "2D/Texture/Texture.h"
#include "3D/Models/ModelEntity.h"

using namespace Aela;

namespace Game {
	class Tile {
		public:
			Tile() {
				type = 0;
				entity = nullptr;
			}

			Tile(unsigned int type) {
				this->type = type;
				entity = nullptr;
			}

			// Note: if the IDs of the scripts are stored as nothing (""), then these functions
			// return nullptr. Otherwise, they return a pointer to the name of the script, which
			// could be long (since there could be thousands of scripts).
			std::string* getWalkedOnScriptID(), *getPromptedScriptID();

			size_t getType();
			void setType(size_t type);
			ModelEntity* getEntity();
			void setEntity(ModelEntity* entity);
			void setWalkedOnScript(std::string name);
			void setPromptedScript(std::string name);

		private:
			// These are the names of scripts that occur when the tile was walked on and when the tile
			// was in front of the player when the player pressed the "prompt/interact" button. The
			// ScriptManager holds these scripts (obviously).
			std::string walkedOnScriptID = "", promptedScriptID = "";

			// The texture used to represent the tile, which comes from one of the images that contain
			// several textures.
			size_t type;

			ModelEntity* entity;
	};
}