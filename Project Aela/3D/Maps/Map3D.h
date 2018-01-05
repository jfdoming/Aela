/*
* Class: Map3D 3D
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to store the 3D entities of a map/world.
*              Warning! I would highly recommend avoiding models that contains transparency,
*              because having multiples of them in the same frame may cause small glitches!
*              Note that transparent models get rendered after non-transparent models. Only
*              use transparent models if you know that they won't be rendered over one another
*              in a frame. Transparent models aren't outright banned since they can sometimes
*              be necessary in order to perform some optimizations (e.g. a 2D character sprite
*              being rendered on a square, with the sprite being changeable).
*/

#pragma once

#include "../../Resource Management/Resource.h"
#include "../Light/LightEntity.h"
#include "../Models/ModelEntity.h"
#include "../Billboards/BillboardEntity.h"
#include "../Skybox/SkyboxEntity.h"
#include <unordered_map>

using namespace Aela;

class Map3D : public Resource {
	public:
		Map3D(std::string src) : Resource(src) {
		}

		// These allow the user to interact with the map using single entities.
		void addLight(long long key, LightEntity* light), addModel(long long key, ModelEntity* model),
			addModelWithTransparency(long long key, ModelEntity* model), addBillboard(long long key, BillboardEntity* billboard),
			addSkybox(long long key, SkyboxEntity* skybox);
		LightEntity* getLight(long long id);
		ModelEntity* getModel(long long id);
		BillboardEntity* getBillboard(long long id);
		SkyboxEntity* getSkybox(long long id);
		void removeLight(long long id), removeModel(long long id), removeBillboard(long long id), removeSkybox(long long id);
		void removeAllLights(), removeAllModels(), removeAllBillboards(), removeAllSkyboxes();

		// These should only be used in situations such as when the user needs to iterate through the maps of entities.
		std::unordered_map<long long, LightEntity>* getLights();
		std::unordered_map<long long, ModelEntity>* getModels();
		std::unordered_map<long long, BillboardEntity>* getBillboards();
		std::unordered_map<long long, SkyboxEntity>* getSkyboxes();
		std::unordered_map<Model*, std::vector<long long>>* getResourceGroupedModelsWithoutTransparency();
		std::unordered_map<Model*, std::vector<long long>>* getResourceGroupedModelsWithTransparency();

		// Used by the MapLoader.
		void addModelWithoutGeneratingData(long long id, ModelEntity* model);
		void generateAdditionalModelData(long long whichModel, bool containsTransparency);
		void removeAdditionalModelData(bool modelsAreTransparent);

	private:
		// These are the maps of entities in the world.
		// After hours of contemplation and programming, unordered maps turned out to be the best to use.
		std::unordered_map<long long, LightEntity> lights;
		std::unordered_map<long long, ModelEntity> models;
		std::unordered_map<long long, BillboardEntity> billboards;
		std::unordered_map<long long, SkyboxEntity> skyboxes;

		// This stores the model entities based on the model resource that is used. This allows for a beneficial
		// optimization in the renderer. Note that models that contain transparency should be rendered LAST.
		std::unordered_map<Model*, std::vector<long long>> resourceGroupedModelsWithoutTransparency;
		std::unordered_map<Model*, std::vector<long long>> resourceGroupedModelsWithTransparency;

		// These store the keys with the highest values. These are used for determining where to add a new entity using
		// a method that makes adding and removing entities fast, with the only drawback being that a key cannot be
		// reused.
		unsigned int lastLightKey = 0, lastModelKey = 0, lastBillboardKey = 0, lastSkyboxKey = 0;
};
