#pragma once

#include "stdafx.h"
#include "Resource.h"
#include <unordered_map>

namespace Aela {
	class ResourceManager {
		public:
			AELA_EXPORT ResourceManager(int resourceCount);
			AELA_EXPORT ~ResourceManager();

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
			AELA_EXPORT bool loadText(const char * src, bool crucial);

			/*
			* Obtain the specified resource for use in the application.
			*/
			AELA_EXPORT Resource& obtain(const char * src);

			// error handling
			AELA_EXPORT const char * getNewCrucialInvalidResourceKey();
			AELA_EXPORT std::vector<const char *>& getNewInvalidResourceKeys();
		private:
			std::unordered_map<int, int> resources;
			std::vector<const char *> invalidResourceKeys;
			//const char * crucialInvalidResourceKey = "";
	};
}