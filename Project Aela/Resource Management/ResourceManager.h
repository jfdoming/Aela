#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "ResourceLoader.h"
#include <unordered_map>

namespace Aela {
	class ResourceManager {
		public:
			ResourceManager(int resourceCount);
			~ResourceManager();

			/*
			 * The following Resource loading methods have similar parameters and return values.
			 *
			 * @param src the path to the source file (relative to the resource folder for that resource type,
			 * defined in a constant value as TYPE_LOAD_PATH)
			 * @param crucial whether or not this resource is crucial to the application (if this Resource is
			 * loaded in a group and loading of this Resource fails, crucial = true means loading of Resources
			 * in that group will cease and the method will return false, resulting in not all resources being loaded).
			 */

			/*
			 * Read in text from a text file and store it as a Resource, accessible using obtain(string).
			 */
			virtual bool load(std::string src, bool crucial, ResourceLoader * loader) final;

			/*
			* Obtain the specified resource for use in the application.
			*/
			Resource& obtain(std::string src);

			// error handling
			std::string getNewCrucialInvalidResourceKey();
			std::vector<std::string>& getNewInvalidResourceKeys();
		private:
			std::unordered_map<std::string, Resource*> resources;
			std::vector<std::string> invalidResourceKeys;
			std::string crucialInvalidResourceKey = "";
	};
}