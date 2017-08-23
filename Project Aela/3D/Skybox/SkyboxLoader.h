#pragma once

#include "../../2D/Texture/TextureLoader.h"

namespace Aela {
	class SkyboxLoader : public Aela::TextureLoader {
		public:
			static const std::string paths[];

			SkyboxLoader();
			virtual ~SkyboxLoader();

			virtual void expose(LuaManager& mgr);

			SkyboxLoader(SkyboxLoader const&) = delete;
			void operator=(SkyboxLoader const&) = delete;

			virtual bool load(ResourceMap& resources, std::string src);
	};
}
