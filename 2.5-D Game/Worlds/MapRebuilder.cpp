#include "MapRebuilder.h"
#include "../Utilities/MathConstants.h"

Game::MapRebuilder::MapRebuilder() {
	chunkRenderDistances = glm::vec3(1, 1, 1);
}

void Game::MapRebuilder::rebuildMap(World* world, size_t currentWorldID, CharacterTracker* characterTracker) {
	// Clean the map here (by removing entities and such).
	map->removeAllModels();

	Location* playerLocation = playerCharacter->getLocation();
	glm::ivec3 playerTile = playerLocation->getTile();
	glm::ivec2 playerChunk = playerLocation->getChunk();
	for (double z = playerChunk.y - chunkRenderDistances.z; z <= playerChunk.y + chunkRenderDistances.z; z++) {
		for (double x = playerChunk.x - chunkRenderDistances.x; x <= playerChunk.x + chunkRenderDistances.x; x++) {
			glm::ivec2 chunkCoord = glm::ivec2(x, z);
			Chunk* chunk = world->getChunk(chunkCoord);

			if (chunk != nullptr) {
				glm::vec2 chunkPositionOnMap;
				chunkPositionOnMap.x = (float) ((-chunkRenderDistances.x) + x + 1) * CHUNK_WIDTH + 0.5f;
				chunkPositionOnMap.y = (float) ((-chunkRenderDistances.z) + z + 1) * CHUNK_LENGTH + 0.5f;

				// In addition to adding tiles to the world, characters must also be added.
				auto charactersPointer = characterTracker->getCharactersInChunk(currentWorldID, chunkCoord);
				if (charactersPointer != nullptr) {
					auto characters = *charactersPointer;
					for (auto iter : characters) {
						Character* character = characterTracker->getCharacterByID(iter.second);

						// Note that characters are represented by models.
						ModelEntity modelEntity;
						size_t entityInMap;
						glm::ivec3 tileOfChunk = character->getLocation()->getTile();

						// The position of the character on the map. The distance the character is character->getModeltranslated up from its
						// tileCoord is sin(PI/6) * 0.5 + 0.05;
						glm::vec3 characterPositionOnMap((float)(chunkPositionOnMap.x + tileOfChunk.x),
							(float)(tileOfChunk.y + 0.3f), (float)(chunkPositionOnMap.y + tileOfChunk.z));

						modelEntity.setPosition(characterPositionOnMap);
						modelEntity.setRotation((float)ELEVEN_SIXTHS_PI, 0, 0);
						modelEntity.setVisibility(character->isVisible());
						modelEntity.setModel(character->getModel());
						entityInMap = map->getModels()->size();
						map->addModelWithTransparency(entityInMap, &modelEntity);
						ModelEntity* newEntity = map->getModel(entityInMap);
						character->setEntity(newEntity);

						// Make sure to update the character's animation's ModelEntity pointer.
						AnimationTrack3D* track3D = animator->get3DTrack(character->getName() + "/mv");
						if (track3D != nullptr) {
							for (auto& pair : *track3D->getKeyFrames()) {
								pair.second.setObject(newEntity);
							}
						}

						AnimationTrackModel* trackModel = animator->getModelTrack(character->getName() + "/st");
						if (trackModel != nullptr) {
							for (auto& pair : *trackModel->getKeyFrames()) {
								pair.second.setModelEntity(newEntity);
							}
						}
					}
				}

				size_t counter = 0;
				for (auto tilePair : *chunk->getTiles()) {
					Tile* tile = chunk->getTile(tilePair.first);

					// Note: a tileCoord type of zero signifies a blank space.
					if (tile != nullptr && tile->getType() != 0) {
						// Note that tiles are represented by models.
						ModelEntity modelEntity;
						size_t entityInMap;
						glm::vec3 tilePositionOnMap;
						std::string textureName;

						tilePositionOnMap.x = (float)(chunkPositionOnMap.x + tilePair.first.x);
						tilePositionOnMap.y = (float)tilePair.first.y;
						tilePositionOnMap.z = (float)(chunkPositionOnMap.y + tilePair.first.z);

						switch (tileAtlas->getTileType(tile->getType())->getBehaviour()) {
						case TileBehaviour::FLOOR:
							break;
						case TileBehaviour::RAMP_RIGHT:
							tilePositionOnMap.y += 0.5f;
							modelEntity.setRotation((float)-QUARTER_PI, (float)THREE_HALVES_PI, 0);
							modelEntity.setScaling((float)ROOT_OF_TWO, (float)ROOT_OF_TWO, 1);
							break;
						case TileBehaviour::RAMP_UP:
							tilePositionOnMap.y += 0.5f;
							modelEntity.setRotation((float)-QUARTER_PI, 0, 0);
							modelEntity.setScaling(1, (float)ROOT_OF_TWO, (float)ROOT_OF_TWO);
							break;
						case TileBehaviour::RAMP_LEFT:
							tilePositionOnMap.y += 0.5f;
							modelEntity.setRotation((float)-QUARTER_PI, (float)HALF_PI, 0);
							modelEntity.setScaling((float)ROOT_OF_TWO, (float)ROOT_OF_TWO, 1);
							break;
						case TileBehaviour::RAMP_DOWN:
							tilePositionOnMap.y += 0.5f;
							modelEntity.setRotation((float)-QUARTER_PI, (float)PI, 0);
							modelEntity.setScaling(1, (float)ROOT_OF_TWO, (float)ROOT_OF_TWO);
							break;
						case TileBehaviour::RAMP_UP_RIGHT_DEPRESSED:
							modelEntity.setRotation(0, 0, 0);
							modelEntity.setScaling(1, 1, 1);
							break;
						case TileBehaviour::RAMP_UP_LEFT_DEPRESSED:
							modelEntity.setRotation(0, (float)HALF_PI, 0);
							break;
						case TileBehaviour::RAMP_DOWN_LEFT_DEPRESSED:
							modelEntity.setRotation(0, (float)PI, 0);
							break;
						case TileBehaviour::RAMP_DOWN_RIGHT_DEPRESSED:
							modelEntity.setRotation(0, (float)THREE_HALVES_PI, 0);
							break;
						case TileBehaviour::RAMP_UP_RIGHT_ELEVATED:
							modelEntity.setRotation(0, 0, 0);
							break;
						case TileBehaviour::RAMP_UP_LEFT_ELEVATED:
							modelEntity.setRotation(0, (float)HALF_PI, 0);
							break;
						case TileBehaviour::RAMP_DOWN_LEFT_ELEVATED:
							modelEntity.setRotation(0, (float)PI, 0);
							break;
						case TileBehaviour::RAMP_DOWN_RIGHT_ELEVATED:
							modelEntity.setRotation(0, (float)THREE_HALVES_PI, 0);
							break;
						case TileBehaviour::BOX:
							break;
						case TileBehaviour::WALL_RIGHT:
							modelEntity.setRotation(0, (float) -HALF_PI, 0);
							break;
						case TileBehaviour::WALL_FRONT:
							modelEntity.setRotation(0, (float) 0, 0);
							break;
						case TileBehaviour::WALL_LEFT:
							modelEntity.setRotation(0, (float) HALF_PI, 0);
							break;
						default:
							// This type is unknown.
							continue;
						}

						modelEntity.setPosition(tilePositionOnMap);
						modelEntity.setModel(tileAtlas->getTileModel(tile->getType()));
						entityInMap = map->getModels()->size();
						map->addModel(entityInMap, &modelEntity);
						tile->setEntity(map->getModel(entityInMap));
					}
				}
			}
		}
	}


	// This sets the loaded Map3D back up. Note that the Map3D's file is nearly blank.
	/*for (auto chunkPair : *world->getChunks()) {
		Chunk* chunk = world->getChunk(glm::ivec2(chunkPair.first.x, chunkPair.first.y));

		
	}*/

	// The animator must be updated in order to make sure that entity transformations are correct.
	animator->update();
}

void Game::MapRebuilder::bindMap(Map3D* map) {
	this->map = map;
}

void Game::MapRebuilder::setAnimator(Animator* animator) {
	this->animator = animator;
}

void Game::MapRebuilder::setTileAtlas(TileAtlas* tileAtlas) {
	this->tileAtlas = tileAtlas;
}

void Game::MapRebuilder::setChunkRenderingDistances(glm::vec3 chunkRenderDistances) {
	this->chunkRenderDistances = chunkRenderDistances;
}

void Game::MapRebuilder::setPlayerCharacter(Character* playerCharacter) {
	this->playerCharacter = playerCharacter;
}
