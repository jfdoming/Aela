/*
* Class: Character Tracker
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to store and provide characters.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "../../Project Aela/Menus/RectComponent.h"
#include "../../Project Aela/Utilities/glmut.h"
#include "../Character/Character.h"
#include "../Worlds/World.h"
#include "../Save States/Saveable.h"
#include <unordered_map>

#define PLAYER_NAME "player"

namespace Game {
	// The following represents a map of characters, in which characters are organized by map, chunk and position in 
	// their chunk.
	typedef std::unordered_map<size_t, std::unordered_map<glm::ivec2, std::unordered_map<glm::ivec3, size_t,
		IVec3HashMapFunctions, IVec3HashMapFunctions>, IVec2HashMapFunctions, IVec2HashMapFunctions>> CharacterMap;

	class CharacterProvider : public Saveable {
		friend class Character;
		public:
			CharacterProvider();
			~CharacterProvider();

			void update();

			// After all characters have been added, this will generate their models.
			void generateCharacterModelsForAllCharacters();

			// These are adding and getting character functions.
			bool addCharacter(Character* character);
			bool addCharacter(Character* character, size_t* id);
			bool removeCharacterByID(size_t id);
			Character* getCharacterByID(size_t id);
			Character* getCharacterByName(std::string name);
			Character* getCharacterByLocation(const Location& location);
			std::unordered_map<glm::ivec3, size_t, IVec3HashMapFunctions, IVec3HashMapFunctions>*
				getCharactersInChunk(size_t world, glm::ivec2 chunk);
			
			void saveDataToSaveState(SaveState* saveState) override;
			void loadDataFromSaveState(SaveState* saveState) override;

		private:
			// This is a map of characters, sorted by their unique IDs.
			std::unordered_map<size_t, Character*> characters;
			size_t nextCharacterID = 0;

			// These maps store characters based on their other properties so that the characters may be accessed by
			// a specific property quickly.
			std::unordered_map<std::string, size_t> charactersByName;
			CharacterMap charactersByLocation;

			void characterWasMoved(std::string name, Location* oldLocation, Location* newLocation);
			void generateCharacterModel(size_t character);
	};
}