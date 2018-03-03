/*
* Class: Character Manager
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage characters.
*/

#pragma once
#include <unordered_map>
#include "Utilities/glmut.h"
#include "../Character/Character.h"
#include "../Scripts/ScriptManager.h"
#include "../Worlds/World.h"
#include "Resource Management/ResourceManager.h"
#include "Animation/Animator.h"
#include "3D/Animation/AnimationTrack3D.h"
#include "3D/Camera/Camera3D.h"

#define PLAYER_NAME "player"

namespace Game {
	// The following represents a map of characters, in which characters are organized by map, chunk and position in 
	// their chunk.
	typedef std::unordered_map<size_t, std::unordered_map<glm::ivec2, std::unordered_map<glm::ivec3, size_t,
		IVec3HashMapFunctions, IVec3HashMapFunctions>, IVec2HashMapFunctions, IVec2HashMapFunctions>> CharacterMap;

	class CharacterManager {
		public:
			CharacterManager() {}

			void setup(ResourceManager* resourceManager, Animator* animator, Camera3D* camera, TimeManager* timeManager,
				ScriptManager* scriptManager);
			void update();

			// After all characters have been added, this will generate their models.
			void generateCharacterModels(ResourceManager* resourceManager);

			// These are adding and getting character functions.
			bool addCharacter(Character* character, size_t* id);
			bool removeCharacterByID(size_t id);
			Character* getCharacterByID(size_t id);
			Character* getCharacterByName(std::string name);
			Character* getCharacterByLocation(Location* location);
			std::unordered_map<glm::ivec3, size_t, IVec3HashMapFunctions, IVec3HashMapFunctions>*
				getCharactersInChunk(size_t world, glm::ivec2 chunk);

			// These are movement-related functions.
			void turn(Character* character, TileDirection direction);
			void turn(size_t id, TileDirection direction);
			void turn(std::string name, TileDirection direction);
			void move(Character* character, TileDirection direction, std::string scriptOnCompletion);
			void move(size_t id, TileDirection direction, std::string scriptOnCompletion);
			void move(std::string name, TileDirection direction, std::string scriptOnCompletion);
			void stopMoving(size_t id);
			void stopMoving(std::string name);

			// I don't know what else to call these...
			bool doesMapNeedToBeRebuilt();
			void mapWasRebuilt();

		private:
			// This is a map of characters, sorted by their unique IDs.
			std::vector<Character> characters;

			// These maps store characters based on their other properties so that the characters may be accessed by
			// a specific property quickly.
			std::unordered_map<std::string, size_t> charactersByName;
			CharacterMap charactersByLocation;

			ResourceManager* resourceManager;
			Animator* animator;
			Camera3D* camera;
			ScriptManager* scriptManager;

			// Used to tell the WorldManager if the Map3D needs to be cleaned and rebuilt.
			bool mapNeedsToBeRebuilt = false;

			void animateCharacterMovement(Character* character, glm::ivec3 translation, std::string scriptOnCompletion);
			void animateCamera(glm::vec3 translation, float speed, long long timeToAdvanceTrackBy);
	};
}