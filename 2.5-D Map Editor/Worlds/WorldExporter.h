/*
* Class: World Exporter
* Author: Robert Ciborowski
* Date: 05/05/2018
* Description: A class used to export worlds.
*/

#pragma once
#include "World.h"

namespace Game {
	class WorldExporter {
		public:
			WorldExporter();

			bool exportWorld(std::string path, World* world);
	};
}