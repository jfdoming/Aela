#pragma once

#include "stdafx.h"
#include "../Lua/LuaManager.h"
#include "Resource.h"
#include "ResourceQuery.h"
#include "ResourceLoader.h"
#include <unordered_map>
#include <vector>

#include "OBJLoader.h"
#include "TextureLoader.h"

namespace Aela {
	class ResourceManager {
		public:
			static OBJLoader OBJECT_LOADER;
			static TextureLoader TEXTURE_LOADER;

			enum class Status {
				OK, FAILED, ABORT
			};

			ResourceManager(int resourceCount);
			~ResourceManager();

			void addToLuaInstance(LuaManager& mgr);

			/*
			 * The following Resource loading methods have similar parameters and return values.
			 *
			 * @param src the path to the source file (relative to the resource folder for that resource type,
			 * defined in a constant value as TYPE_LOAD_PATH)
			 * @param crucial whether or not this resource is crucial to the application (if this Resource is
			 * loaded in a group and loading of this Resource fails, crucial = true means loading of Resources
			 * in that group will cease and the method will return false, resulting in not all resources being loaded).
			 */

			void bindLoader(ResourceLoader* loader);
			void bindGroup(std::string group);

			Status loadGroup(std::string name);
			Status unloadGroup(std::string name);
			void addToGroup(std::string src, bool crucial);
			void addToGroup(ResourceQuery& query);

			/*
			* Read from a file and store it as a Resource, accessible using obtain(string).
			*/
			Status load(std::string src, bool crucial, ResourceLoader& loader);

			/*
			* Read from a file and store it as a Resource, accessible using obtain(string).
			*/
			Status load(ResourceQuery& query);

			/*
			* Unload the specified resource.
			*/
			void unload(std::string src);

			/*
			* Obtain the specified resource for use in the application.
			*/
			template <class T> T* obtain(std::string src) {
				return (T*) obtain_impl(src);
			}

			// error handling
			std::string getNewCrucialInvalidResourceKey();
			std::vector<std::string>& getNewInvalidResourceKeys();
		private:
			std::unordered_map<std::string, Resource*> resources;
			std::unordered_map<std::string, std::vector<ResourceQuery>> groups;

			std::vector<std::string> invalidResourceKeys;
			std::string crucialInvalidResourceKey = "";

			ResourceLoader* boundLoader;
			std::vector<ResourceQuery>* boundGroup;

			Resource* obtain_impl(std::string src);
	};
}