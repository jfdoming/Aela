/*
* Class: Material Loader
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for loading material files.
*/

#pragma once

#include "ResourceLoader.h"

namespace Aela {
	class MaterialLoader : public ResourceLoader {
		public:
			static MaterialLoader& getInstance() {
				// This is guaranteed to be destroyed, and instantiated on first use.
				static MaterialLoader instance;
				return instance;
			}

			MaterialLoader(MaterialLoader const&) = delete;
			void operator=(MaterialLoader const&) = delete;

			virtual Resource* load(std::ifstream& in);
		private:
			MaterialLoader() {}

			GLuint loadDDSToGLuint(std::string filePath);
	};
}