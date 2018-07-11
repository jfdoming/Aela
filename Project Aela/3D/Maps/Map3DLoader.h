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
#include "../../Renderer/GLRenderer.h"

namespace Aela {
	class Map3DLoader : public ResourceLoader {
		public:
			Map3DLoader(std::string resourceRoot);
			virtual ~Map3DLoader();

			virtual bool load(ResourceMap& resources, std::string src);

			void bindRenderer(GLRenderer* renderer);

		private:
			// TODO make it so that this class does not require a renderer ptr
			GLRenderer* renderer;

			// Because a map refers to models, this loader needs to know what the resource root is.
			// Maybe resource root should be part of ResourceLoader because of this. To be honest,
			// adding a resource root seemed to make everything more complicated.
			std::string resourceRoot;
	};
}