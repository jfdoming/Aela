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

#include <map>

using namespace Aela;

class Map3D : public Resource {
	public:
		Map3D() {

		}

		// These are the getters and setters of the class. When loading a map, it is better to load the entities directly into the map using
		// something such as "map3D.getLights()->push_back()" rather than creating seperate std::maps for those entities and using the set functions.
		void setLights(std::map<int, LightEntity>* lights);
		std::map<int, LightEntity>* getLights();
		void setModels(std::map<int, ModelEntity>* models);
		std::map<int, ModelEntity>* getModels();
		void setBillboards(std::map<int, BillboardEntity>* billboards);
		std::map<int, BillboardEntity>* getBillboards();

	private:
		// These are the maps of entities in the world.
		std::map<int, LightEntity> lights;
		std::map<int, ModelEntity> models;
		std::map<int, BillboardEntity> billboards;
};
