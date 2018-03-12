/*
* Class: ResourceManager
* Author: Julian Dominguez-Schatz
* Date: 05/06/2017
* Description: Manages all resources used in the application.
*/

#pragma once

#include "stdafx.h"
#include "../Lua/LuaManager.h"
#include "../Lua/Exposable.h"
#include "Resource.h"
#include "ResourceQuery.h"
#include "ResourceGroup.h"
#include "ResourceMap.h"
#include "ResourceLoader.h"
#include <unordered_map>
#include <vector>

namespace Aela {
	class ResourceManager: public Exposable {
		public:
			enum class Status {
				OK, FAILED, ABORT
			};


			ResourceManager(int resourceCount);
			~ResourceManager();

			void expose(LuaManager& mgr);

			void bindLoader(ResourceLoader* loader);
			void bindGroup(std::string group);

			Status loadGroup(std::string name);
			Status unloadGroup(std::string name);
			void addToGroup(std::string src, bool crucial);
			void addToGroup(ResourceQuery& query);

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
			* Read from a file and store it as a Resource, accessible using obtain(string).
			*/
			Status load(std::string src, bool crucial, ResourceLoader& loader);

			/*
			* Read from a file and store it as a Resource, accessible using obtain(string).
			*/
			Status load(ResourceQuery& query);

			template <class T> bool obtain(std::string src, T*& result) {
				return resources.get(src, result);
			}

			/*
			* Unload the specified resource.
			*/
			void unload(std::string src);

			void setResourceRoot(std::string resourceRoot);

			// error handling
			std::string getNewCrucialInvalidResourceKey();
			std::vector<std::string>& getNewInvalidResourceKeys();
		private:
			std::unordered_map<std::string, ResourceGroup> groups;
			ResourceMap resources;

			std::string resourceRoot = "../../";

			std::vector<std::string> invalidResourceKeys;
			std::string crucialInvalidResourceKey = "";

			ResourceLoader* boundLoader;
			ResourceGroup* boundGroup;
	};
}