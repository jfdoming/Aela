#include "MapRebuilder.h"
#include "../Player/Player.h"
#include "WorldManager.h"
#include "../Character/CharacterProvider.h"
#include "../Utilities/MathConstants.h"
#include "../../Project Aela/Renderer/GLRenderer.h"

Game::MapRebuilder::MapRebuilder() {
	chunkRenderDistances = glm::vec3(1, 1, 1);
	animator = GameObjectProvider::getAnimator();
	renderer = GameObjectProvider::getRenderer();
}

void Game::MapRebuilder::setup() {
	tileAtlas = GameObjectProvider::getTileAtlas();
	playerCharacter = GameObjectProvider::getPlayer()->getCharacter();
	worldManager = GameObjectProvider::getWorldManager();
	characterProvider = GameObjectProvider::getCharacterProvider();
}

void Game::MapRebuilder::scenesWereSetup() {
	gameplayScene = GameObjectProvider::getGameplayScene();
}

void Game::MapRebuilder::rebuildMap(World* world) {
	size_t currentWorldID = playerCharacter->getLocation()->getWorld();
	map = worldManager->getMap3D();
	gameplayScene->setMap(map);

	// Clean the map here (by removing entities and such).
	map->removeAllModels();

	if (world->isUsingLights()) {
		renderer->activateFeature(RendererFeature::LIGHTS);
		renderer->activateFeature(RendererFeature::SHADOWS);
	} else {
		renderer->deactivateFeature(RendererFeature::LIGHTS);
		renderer->deactivateFeature(RendererFeature::SHADOWS);
	}

	// This creates transparency layers.
	map->addModelTransparencyLayer(0);
	map->addModelTransparencyLayer(1);

	Location* playerLocation = playerCharacter->getLocation();
	glm::ivec3 playerTile = playerLocation->getTileGroup();
	glm::ivec2 playerChunk = playerLocation->getChunk();

	for (double z = playerChunk.y - chunkRenderDistances.z; z <= playerChunk.y + chunkRenderDistances.z; z++) {
		for (double x = playerChunk.x - chunkRenderDistances.x; x <= playerChunk.x + chunkRenderDistances.x; x++) {
			glm::ivec2 chunkCoord = glm::ivec2(x, z);
			Chunk* chunk = world->getChunk(chunkCoord);
			glm::vec2 chunkPositionOnMap;
			chunkPositionOnMap.x = (float) ((-chunkRenderDistances.x) + x + 1) * CHUNK_WIDTH + 0.5f;
			chunkPositionOnMap.y = (float) ((-chunkRenderDistances.z) + z + 1) * CHUNK_LENGTH + 0.5f;
			// In addition to adding tiles to the world, characters must also be added.
			auto charactersPointer = characterProvider->getCharactersInChunk(currentWorldID, chunkCoord);
			if (charactersPointer != nullptr) {
				auto characters = *charactersPointer;
				for (auto& iter : characters) {
					Character* character = characterProvider->getCharacterByID(iter.second);

					// Note: even if the character is invisible, we should still construct
					// a new ModelEntity for it (especially if you don't want the teleporting
					// animation to break).
					if (character->isAlive()/* && character->isVisible()*/) {
						// Note that characters are represented by models.
						ModelEntity modelEntity;
						size_t entityInMap;
						glm::ivec3 tileOfChunk = character->getLocation()->getTileGroup();

						// The position of the character on the map. The distance the character is character->getModeltranslated up from its
						// tileCoord is sin(PI/6) * 0.5 + 0.05;
						glm::vec3 characterPositionOnMap((float) (chunkPositionOnMap.x + tileOfChunk.x),
							(float) (tileOfChunk.y + characterYOffsetInWorldspace), (float) (chunkPositionOnMap.y + tileOfChunk.z));

						modelEntity.setPosition(characterPositionOnMap);
						modelEntity.setRotation((float) ELEVEN_SIXTHS_PI, 0, 0);
						modelEntity.setVisibility(character->isVisible());
						modelEntity.setModel(character->getModel());
						entityInMap = map->getModels()->size();
						map->addModelWithTransparency(entityInMap, &modelEntity, CHARACTER_LAYER);
						ModelEntity* newEntity = map->getModel(entityInMap);
						character->setEntity(newEntity);
						newEntity->setVisibility(character->isVisible());

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
			}

			if (chunk != nullptr) {
				// This stores the ModelEntities used for glass and adds them to the map last (so that they are rendered last).
				std::vector<std::pair<ModelEntity, Tile&>> glassModelEntities;

				for (auto& tileGroupPair : *chunk->getTileGroups()) {
					TileGroup* tileGroup = chunk->getTileGroup(tileGroupPair.first);

					if (tileGroup != nullptr) {
						for (auto& tilePair : *tileGroup->getTiles()) {
							Tile& tile = tilePair.second;

							// Note: a tile type of zero signifies a blank space.
							if (tile.getType() != 0) {
								// Note that tiles are represented by models.
								ModelEntity modelEntity;
								size_t entityInMap;
								glm::vec3 tilePositionOnMap;
								std::string textureName;
								bool isGlass = false;

								tilePositionOnMap.x = (float) (chunkPositionOnMap.x + tileGroupPair.first.x);
								tilePositionOnMap.y = (float) tileGroupPair.first.y;
								tilePositionOnMap.z = (float) (chunkPositionOnMap.y + tileGroupPair.first.z);

								switch (tileAtlas->getTileType(tile.getType())->getShape()) {
									case TileShape::FLOOR:
										// tilePositionOnMap.y += 1;
										break;
									case TileShape::BOXED_FLOOR:
										break;
									case TileShape::LIQUID_FLOOR:
										tilePositionOnMap.y -= 0.2f;
										break;
									case TileShape::RAMP_RIGHT:
										tilePositionOnMap.y += 0.5f;
										modelEntity.setRotation((float) -QUARTER_PI, (float) THREE_HALVES_PI, 0);
										modelEntity.setScaling((float) ROOT_OF_TWO, (float) ROOT_OF_TWO, 1);
										break;
									case TileShape::RAMP_UP:
										tilePositionOnMap.y += 0.5f;
										modelEntity.setRotation((float) -QUARTER_PI, 0, 0);
										modelEntity.setScaling(1, (float) ROOT_OF_TWO, (float) ROOT_OF_TWO);
										break;
									case TileShape::RAMP_LEFT:
										tilePositionOnMap.y += 0.5f;
										modelEntity.setRotation((float) -QUARTER_PI, (float) HALF_PI, 0);
										modelEntity.setScaling((float) ROOT_OF_TWO, (float) ROOT_OF_TWO, 1);
										break;
									case TileShape::RAMP_DOWN:
										tilePositionOnMap.y += 0.5f;
										modelEntity.setRotation((float) -QUARTER_PI, (float) PI, 0);
										modelEntity.setScaling(1, (float) ROOT_OF_TWO, (float) ROOT_OF_TWO);
										break;
									case TileShape::RAMP_UP_RIGHT_DEPRESSED:
										modelEntity.setRotation(0, 0, 0);
										modelEntity.setScaling(1, 1, 1);
										break;
									case TileShape::RAMP_UP_LEFT_DEPRESSED:
										modelEntity.setRotation(0, (float) HALF_PI, 0);
										break;
									case TileShape::RAMP_DOWN_LEFT_DEPRESSED:
										modelEntity.setRotation(0, (float) PI, 0);
										break;
									case TileShape::RAMP_DOWN_RIGHT_DEPRESSED:
										modelEntity.setRotation(0, (float) THREE_HALVES_PI, 0);
										break;
									case TileShape::RAMP_UP_RIGHT_ELEVATED:
										modelEntity.setRotation(0, 0, 0);
										break;
									case TileShape::RAMP_UP_LEFT_ELEVATED:
										modelEntity.setRotation(0, (float) HALF_PI, 0);
										break;
									case TileShape::RAMP_DOWN_LEFT_ELEVATED:
										modelEntity.setRotation(0, (float) PI, 0);
										break;
									case TileShape::RAMP_DOWN_RIGHT_ELEVATED:
										modelEntity.setRotation(0, (float) THREE_HALVES_PI, 0);
										break;
									case TileShape::BOX:
										break;
									case TileShape::WALL_RIGHT:
										tilePositionOnMap.x++;
										modelEntity.setRotation(0, (float) -HALF_PI, 0);
										break;
									case TileShape::WALL_FRONT:
										tilePositionOnMap.z--;
										modelEntity.setRotation(0, (float) 0, 0);
										break;
									case TileShape::WALL_LEFT:
										tilePositionOnMap.x--;
										modelEntity.setRotation(0, (float) HALF_PI, 0);
										break;
									case TileShape::GLASS_RIGHT:
										tilePositionOnMap.x++;
										modelEntity.setRotation(0, (float) -HALF_PI, 0);
										isGlass = true;
										break;
									case TileShape::GLASS_FRONT:
										tilePositionOnMap.z--;
										modelEntity.setRotation(0, (float) 0, 0);
										isGlass = true;
										break;
									case TileShape::GLASS_LEFT:
										tilePositionOnMap.x--;
										modelEntity.setRotation(0, (float) HALF_PI, 0);
										isGlass = true;
										break;
									default:
										// This type is unknown.
										continue;
								}

								modelEntity.setPosition(tilePositionOnMap);
								modelEntity.setModel(tileAtlas->getTileModel(tile.getType()));

								if (isGlass) {
									glassModelEntities.push_back(std::pair<ModelEntity, Tile&>(modelEntity, tile));
								} else {
									entityInMap = map->getModels()->size();
									map->addModel(entityInMap, &modelEntity);
									tile.setEntity(map->getModel(entityInMap));
								}
							}
						}
					}
				}

				for (auto& pair : glassModelEntities) {
					size_t entityInMap = map->getModels()->size();
					map->addModelWithTransparency(entityInMap, &pair.first, GLASS_LAYER);
					pair.second.setEntity(map->getModel(entityInMap));
				}
			}
		}
	}

	// The animator must be updated in order to make sure that entity transformations are correct.
	animator->update();
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

glm::ivec3* Game::MapRebuilder::getChunkRenderDistances() {
	return &chunkRenderDistances;
}

float Game::MapRebuilder::getCharacterYOffsetInWorldspace() {
	return characterYOffsetInWorldspace;
}
