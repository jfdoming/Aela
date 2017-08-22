/*
* Class: Map3D Loader
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to load a map file into a Map3D.
*/

#pragma once

#include "Map3D.h"
#include "../../Resource Management/ResourceLoader.h"
#include "../../Resource Management/ResourceMap.h"
#include "../../Renderer/Renderer.h"

namespace Aela {
	class Map3DLoader : public ResourceLoader {
		public:
			Map3DLoader();
			virtual ~Map3DLoader();

			virtual bool load(ResourceMap& resources, std::string src);

			void bindRenderer(Renderer* renderer);

		private:
			Renderer* renderer;
			void setVec3UsingString(std::string* value, glm::vec3* vec3);

			const std::string defaultSkyboxPath = "res/skyboxes/";
			const std::string defaultModelPath = "res/models/";
			const std::string defaultBillboardPath = "res/textures/";
	};
}