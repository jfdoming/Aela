/*
* Class: Material Loader
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for loading material files.
*/

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

#include "MaterialLoader.h"
#include "Material.h"
#include "../../Error Handler/ErrorHandler.h"

#include "../../Utilities/strut.h"

using namespace Aela;

MaterialLoader::MaterialLoader() {
}

MaterialLoader::~MaterialLoader() {
}

void MaterialLoader::expose(LuaManager& mgr) {
	// only expose part of the class to Lua
	luabridge::getGlobalNamespace(mgr.getLuaState())
		.beginClass<Aela::MaterialLoader>("MaterialLoader")
		.endClass();

	// Sorry Waseef, Julian had to comment this out in order to compile.
	// mgr.exposeObject(this, "textureLoader");
}

bool Aela::MaterialLoader::load(ResourceMap& resources, std::string src) {
	Material* material = nullptr;
	std::string line;

	std::ifstream in(src, std::ios::binary);

	if (!isValid(in)) {
		return false;
	}

	// This begins reading the file.
	if (in.is_open()) {
		while (std::getline(in, line)) {
			if (line.find("newmtl ") != std::string::npos) {
				material = new Material();
				std::string name = line.substr(MATERIAL_NAME_START_POSITION, line.size() - MATERIAL_NAME_START_POSITION);
				resources.put(name, material);
			} else if (line.find("map_Kd ") != std::string::npos || line.find("map_Ka ") != std::string::npos) {
				if (material == nullptr) {
					// Error! The file did not specify a material name before setting the texture!
					return false;
				}

				// If this part of the code is reached, the line may look like "map_Kd tire_1.dds".
				std::string fileName = line.substr(MATERIAL_PATH_START_POSITION, line.size() - MATERIAL_PATH_START_POSITION);
				trim(fileName);

				// This retrieves the folder path.
				std::string path = "/";
				for (int i = src.length() - 1; i >= 0; i--) {
					if (src.at(i) == '/') {
						path = src.substr(0, i + 1);
						break;
					}
				}
				
				std::string textureName = path + fileName;
				Texture* texture;
				if (TextureLoader::loadTexture(texture, textureName)) {
					material->setTexture(texture);
				}
			} else {
				// The line in the file is treated as a comment if this line is reached.
			}
		}
	} else {
		AelaErrorHandling::windowError("Aela Material Loader", "The program tried to load a non-existent mtl file.");
		in.close();
		return false;
	}
	in.close();
	return true;
}