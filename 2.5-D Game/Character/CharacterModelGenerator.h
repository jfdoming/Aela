/*
* Class: Character Model Generator
* Author: Robert Ciborowski
* Date: 27/12/2017
* Description: A class for setting up the models of characters by using a template model.
*              Note: if you want to access these character models later via the resource manager, ask
*              for a resource with an src of "ch_X_mo" (where X is the character's texture name).
*
*              This class needs to extend ResourceLoader so that it can add resources to the
*              resource management system.
*/

#pragma once

#include "3D/Models/Model.h"
#include "Resource Management/ResourceLoader.h"
#include "Resource Management/ResourceMap.h"
#include "Resource Management/ResourceManager.h"
#include "Character.h"
#include <map>

namespace Game {
	class CharacterModelGenerator : public ResourceLoader {
		public:
		CharacterModelGenerator() {}
			virtual ~CharacterModelGenerator() {}

			// This will load each character's assets.
			virtual bool load(ResourceMap& resources, std::string src);

			// Before loading, these need to be called.
			void setCharacters(std::vector<Character>* characters);
			void setTemplateModelSource(std::string templateModelSource);
			unsigned int getNumberOfCharactersLoaded();

		private:
			ResourceManager* resourceManager;
			std::vector<Character>* characters;
			unsigned int charactersLoaded = 0;
			std::string templateModelSource;
	};
}