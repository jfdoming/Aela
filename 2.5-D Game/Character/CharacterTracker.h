/*
* Class: Character Tracker
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to keep track of characters.
*
*              Why do you need to track characters? Well, a lot of their own behaviours can be done by themselves.
*              However, there are some things that should not be done by them since it would require each
*              individual character to have their own pointer to things like Aela objects, etc. This would
*              be a waste of memory. In addition, a CharacterTracker gives its WorldManager the ability to
*              get a character based on a given criteria, such as a coordinate.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "../../Project Aela/Menus/RectComponent.h"
#include "Utilities/glmut.h"
#include "../Character/Character.h"
#include "../Worlds/World.h"
#include <unordered_map>

#define PLAYER_NAME "player"

namespace Game {
	// The following represents a map of characters, in which characters are organized by map, chunk and position in 
	// their chunk.
	typedef std::unordered_map<size_t, std::unordered_map<glm::ivec2, std::unordered_map<glm::ivec3, size_t,
		IVec3HashMapFunctions, IVec3HashMapFunctions>, IVec2HashMapFunctions, IVec2HashMapFunctions>> CharacterMap;

	class CharacterTracker {
		public:
			CharacterTracker();
			~CharacterTracker();

			void setup();
			void update();
			void scenesWereSetUp();

			// After all characters have been added, this will generate their models.
			void generateCharacterModels(ResourceManager* resourceManager);

			// These are adding and getting character functions.
			bool trackCharacter(Character* character, size_t* id);
			bool deleteCharacterByID(size_t id);
			Character* getCharacterByID(size_t id);
			Character* getCharacterByName(std::string name);
			Character* getCharacterByLocation(Location* location);
			std::unordered_map<glm::ivec3, size_t, IVec3HashMapFunctions, IVec3HashMapFunctions>*
				getCharactersInChunk(size_t world, glm::ivec2 chunk);

			// These are movement-related functions.
			void turnTrackedCharacter(size_t id, TileDirection direction);
			void turnTrackedCharacter(std::string name, TileDirection direction);
			void moveTrackedCharacter(size_t id, Movement* movement, std::string scriptOnCompletion);
			void moveTrackedCharacter(std::string name, Movement* movement, std::string scriptOnCompletion);
			void teleportTrackedCharacter(size_t id, Location* location, bool animation);
			void teleportTrackedCharacter(std::string name, Location* location, bool animation);

			// These movement-related functions accept a Character* to a character that is
			// assumed to already be tracked. Doing this is kind of bad design, but it allows
			// for optimization. These functions and this entire class is meant to only be used
			// by a WorldManager anyways, so only the WorldManager will use these.
			void turnTrackedCharacter(Character* character, TileDirection direction);
			void moveTrackedCharacter(Character* character, Movement* movement, std::string scriptOnCompletion);
			void teleportTrackedCharacter(Character* character, Location* location, bool animation);

			// I don't know what else to call these...
			bool doesMapNeedToBeRebuilt();
			void mapWasRebuilt();

			void setPlayer(size_t id);
			bool isPlayerDead();
			void setGameplayMenuItems(std::shared_ptr<RectComponent> deathRect, std::shared_ptr<Label> deathText);

		private:
			// These are obtained from GameObjectProvider.
			Animator* animator;
			Camera3D* camera;
			ResourceManager* resourceManager;
			Time* time;
			Scene* gameplayScene;
			ScriptManager* scriptManager;

			// This is a map of characters, sorted by their unique IDs.
			std::unordered_map<size_t, Character*> characters;
			size_t nextCharacterID = 0;

			// These maps store characters based on their other properties so that the characters may be accessed by
			// a specific property quickly.
			std::unordered_map<std::string, size_t> charactersByName;
			CharacterMap charactersByLocation;

			size_t playerID;
			std::shared_ptr<RectComponent> deathRect;
			std::shared_ptr<Label> deathText;

			std::vector<size_t> charactersToDelete;

			// Used to tell the WorldManager if the Map3D needs to be cleaned and rebuilt.
			bool mapNeedsToBeRebuilt = false;

			bool playerIsDead = false;

			const size_t HALF_TELEPORTATION_DURATION = 1000000000;
			bool lockingCameraToPlayer = true;

			void processCharacterMovement(Character* character, Movement* movement, std::string scriptOnCompletion);
			void animateCamera(glm::vec3 translation, float speed, long long timeToAdvanceTrackBy);
			void animateCharacterDeath(Character* character);
			void animatePlayerDeathScreen();
			void resetCameraPosition();
	};
}