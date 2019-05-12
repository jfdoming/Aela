#include "TileMovement.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Character/Character.h"
#include "../Displays/Hints/HintDisplayDuration.h"
#include "../Displays/Hints/HintDisplay.h"
#include "../Game Mode/GameMode.h"
#include "../Aela Game/AelaGame.h"

Game::TileMovement::TileMovement(TileDirection direction, bool collisionCheck) : WalkingMovement(1.0f, direction, collisionCheck) {
}

Game::TileMovement::TileMovement(TileDirection direction, bool collisionCheck, bool running) : WalkingMovement(1.0f, direction, collisionCheck, running) {
}

Game::TileMovement::~TileMovement() {
}

void Game::TileMovement::start(Character* character) {
	WalkingMovement::start(character);
	float tileRemainder;

	if (direction == TileDirection::FORWARD || direction == TileDirection::BACKWARD) {
		float z = character->getLocation()->getTileGroup().z;
		tileRemainder = z - floor(z);
		if (direction == TileDirection::FORWARD) {
			tileRemainder = 0.5f - tileRemainder;
		} else {
			tileRemainder -= 0.5f;
		}
	} else if (direction == TileDirection::LEFT || direction == TileDirection::RIGHT) {
		float x = character->getLocation()->getTileGroup().x;
		tileRemainder = x - floor(x);
		if (direction == TileDirection::LEFT) {
			tileRemainder = 0.5f - tileRemainder;
		} else {
			tileRemainder -= 0.5f;
		}
	} else if (direction == TileDirection::UP || direction == TileDirection::DOWN) {
		float y = character->getLocation()->getTileGroup().y;
		tileRemainder = y - floor(y);
		if (direction == TileDirection::UP) {
			tileRemainder = 0.5f - tileRemainder;
		} else {
			tileRemainder -= 0.5f;
		}
	} else {
		magnitude = 0;
		return;
	}

	magnitude += tileRemainder;// - 0.5f;
}

void Game::TileMovement::onEnd(Character* character) {
	WalkingMovement::onEnd(character);

	if (character == GameObjectProvider::getPlayer()->getCharacter() && GameObjectProvider::getGame()->getGameMode() == GameMode::MAP_EDITOR) {
		Location* location = character->getLocation();
		glm::ivec2 chunk = location->getChunk();
		glm::ivec3 tileGroup = location->getTileGroup();
		GameObjectProvider::getHintDisplay()->displayHint("World: " + std::to_string(location->getWorld()) + ", Chunk: (" + std::to_string(chunk.x)
			+ ", " + std::to_string(chunk.y) + "), Tile: (" + std::to_string(tileGroup.x) + ", " + std::to_string(tileGroup.y) + ", "
			+ std::to_string(tileGroup.z) + ")", HintDisplayDuration::FOREVER);
	}
}
