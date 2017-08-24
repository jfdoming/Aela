/*
* Class: Map3D 3D
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to store the 3D entities of a map/world.
*/

#pragma once

#include "../../Resource Management/Resource.h"
#include "../Light/LightEntity.h"
#include "../Models/ModelEntity.h"
#include "../Billboards/BillboardEntity.h"
#include "../Skybox/SkyboxEntity.h"

#include <map>

using namespace Aela;

class Map3D : public Resource {
	public:
		Map3D(std::string src) : Resource(src) {
		}

		// These allow the user to interact with the map using single entities.
		int addLight(LightEntity* light), addModel(ModelEntity* model), addBillboard(BillboardEntity* billboard), addSkybox(SkyboxEntity* skybox);
		LightEntity* getLight(int id);
		ModelEntity* getModel(int id);
		BillboardEntity* getBillboard(int id);
		SkyboxEntity* getSkybox(int id);
		int removeLight(int id), removeModel(int id), removeBillboard(int id), removeSkybox(int id);

		// These should only be used in situations such as when the user needs to iterate through the maps of entities.
		std::map<int, LightEntity>* getLights();
		std::map<int, ModelEntity>* getModels();
		std::map<int, BillboardEntity>* getBillboards();
		std::map<int, SkyboxEntity>* getSkyboxes();

	private:
		// These are the maps of entities in the world.
		// Q: Why would you use maps when you can use unordered_maps? Maps are slower!
		// A: Whenever you use unordered_map::erase(), it doesn't actually delete the elements from the container, it just calls
		// the destructor, leaving a blank element in that spot. The size of the container doesn't change, and this can break
		// the program in certain areas. On the other hand, maps actually remove the element entirely, causing their size
		// to change after the call to map::erase().
		std::map<int, LightEntity> lights;
		std::map<int, ModelEntity> models;
		std::map<int, BillboardEntity> billboards;
		std::map<int, SkyboxEntity> skyboxes;
};
