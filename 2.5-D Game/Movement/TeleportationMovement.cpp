#include "TeleportationMovement.h"
#include "../Character/Character.h"
#include "../Worlds/WorldManager.h"
#include "../Scripts/ScriptManager.h"
#include "../Particles/CharacterTeleportParticleEmitter.h"
#include "../Camera/CameraController.h"
#include "../Aela Game/AelaGame.h"
#include "../Audio/GameAudioPlayer.h"

Game::TeleportationMovement::TeleportationMovement(Location& destination, TeleportationAnimation animation) {
	teleportationQueue.push(destination);
	this->animation = animation;
}

Game::TeleportationMovement::~TeleportationMovement() {
}

bool Game::TeleportationMovement::moveCharacter(Character* character) {
	if (!currentTeleportIsSetup && !teleportationQueue.empty()) {
		currentTeleportIsSetup = true;
		Location location = teleportationQueue.front();
		WorldManager* worldManager = GameObjectProvider::getWorldManager();
		ScriptManager* scriptManager = GameObjectProvider::getScriptManager();
		Location oldLocation = *character->getLocation();
		Teleporter* teleporter = worldManager->getTeleporter(location);

		if (teleporter != nullptr) {
			teleportationQueue.push(*teleporter->getDestination());
			// The rest of the teleportations that might still occur are all due to teleporters,
			// so if the original teleportation lands at a teleporter, the next teleportations
			// should have the animation of a teleporter teleportation.
			animation = TeleportationAnimation::RISE;
		}

		auto teleportationEndAction = [this, character, worldManager, scriptManager, location]() mutable {
			TileGroup* tileGroup = worldManager->getTileGroup(location);
			// scriptManager->runScript(scriptOnCompletion);
			currentTeleportIsSetup = false;
			teleportationQueue.pop();

			if (tileGroup == nullptr) {
				return;
			}

			scriptManager->runScript(*tileGroup->getWalkedOnScriptID());

			if (tileGroup->getLiquidFloorTile() != nullptr && GameObjectProvider::getGame()->getGameMode() == GameMode::GAMEPLAY) {
				// The character teleported into a liquid. They must die.
				character->kill();
			}
		};

		// This already occurs elsewhere:
		// newMovementsAreAllowed = false;


		if (animation == TeleportationAnimation::RISE) {
			CameraController* cameraController = GameObjectProvider::getCameraController();
			cameraController->setLockCameraToPlayer(false);

			float offset = GameObjectProvider::getWorldManager()->getCharacterYOffsetInWorldspace();
			auto* particleEmitter = new CharacterTeleportParticleEmitter(GameObjectProvider::getTime());
			particleEmitter->setBaseDistance(oldLocation.getWorldSpaceLocation().y + offset + 10);
			particleEmitter->setBaseSpeed(0.00000001f);
			particleEmitter->setPathOffset(oldLocation.getWorldSpaceLocation().y + offset);
			particleEmitter->setLifeTime(1500000000);
			particleEmitter->setCharacter(character);
			particleEmitter->setLocations(&oldLocation, &location);
			particleEmitter->setSecondaryPathOffset(location.getWorldSpaceLocation().y - offset + 8);

			std::vector<GLTexture*> textures;
			auto* texture = dynamic_cast<GLTexture*>(character->getEntity()->getModel()->getSubModels()->at(0).getMaterial()->getTexture());
			textures.push_back(texture);

			auto halfLifeAction = [this, cameraController, location, character]() mutable {
				GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
				cameraController->setLockCameraToPlayer(true);
				character->changeLocation(location);
				GameObjectProvider::getScriptManager()->runScript(scriptOnHalfLife);
			};

			particleEmitter->setActionOnHalfLife(halfLifeAction);
			particleEmitter->setActionOnEnd(teleportationEndAction);
			particleEmitter->setupParticles(&textures, 1, 1, 1);
			GameObjectProvider::getGameplayScene()->putParticleEmitter(particleEmitter);

			// Since the character's entity will get destroyed and be recreated at the teleport end,
			// so in case the entity is used, this needs to be done:
			character->setEntity(nullptr);
		} else if (animation == TeleportationAnimation::FADE) {
			auto halfwayTeleportAction = [this, character, oldLocation, location]() mutable {
				GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
				character->changeLocation(location);
				GameObjectProvider::getScriptManager()->runScript(scriptOnHalfLife);
			};

			auto audioEvent = []() {
				GameObjectProvider::getGameAudioPlayer()->playAudio("fade");
			};

			GameObjectProvider::getTimer()->scheduleEventInMillis(350, audioEvent);
			GameObjectProvider::getTimer()->scheduleEventInMillis(750, halfwayTeleportAction);
			GameObjectProvider::getTimer()->scheduleEventInMillis(1500, teleportationEndAction);
			GameObjectProvider::getGame()->animateFadeTeleport();
		} else {
			character->changeLocation(location);
			teleportationEndAction();
		}
	} else if (!aboutToEnd && teleportationQueue.empty()) {
		auto event = [this]() {
			complete = true;
		};

		aboutToEnd = true;

		if (animation == TeleportationAnimation::FADE) {
			// If the last teleport is a fade, create a small pause.
			GameObjectProvider::getTimer()->scheduleEventInMillis(FADE_TELEPORT_PAUSE, event);
		} else {
			GameObjectProvider::getTimer()->scheduleEventInMillis(0, event);
		}
	} else {
		return complete;
	}

	return false;
}

void Game::TeleportationMovement::setScriptOnHalfLife(std::string scriptOnHalfLife) {
	this->scriptOnHalfLife = scriptOnHalfLife;
}
