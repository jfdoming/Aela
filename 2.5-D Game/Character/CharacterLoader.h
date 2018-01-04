/*
* Class: Character Loader
* Author: Robert Ciborowski
* Date: 27/12/2017
* Description: A class for setting up the models of characters by using a template model.
*              Note: if you want to access these character models later via the resource manager, ask
*              for a resource with an src of "char_X_model" (where X is the character's texture name).
*/

#pragma once

#include "3D/Models/Model.h"
#include "Resource Management/ResourceLoader.h"
#include "Resource Management/ResourceMap.h"
#include "Resource Management/ResourceManager.h"
#include "Character.h"
#include <map>

namespace Game {
	class CharacterLoader : public ResourceLoader {
		public:
		CharacterLoader() {}
			virtual ~CharacterLoader() {}

			// This will load each character's assets.
			virtual bool load(ResourceMap& resources, std::string src);

			// Before loading, these need to be called.
			void setResourceManager(ResourceManager* resourceManager);
			void setCharactersToLoad(std::vector<Character>* charactersToLoad);

			unsigned int getNumberOfCharactersLoaded();

		private:
			ResourceManager* resourceManager;
			std::vector<Character>* charactersToLoad;
			unsigned int charactersLoaded = 0;
	};
}