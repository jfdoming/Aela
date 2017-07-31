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

// This is all temporary until everything is restructured so that we only have one DDS loader!
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

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

bool Aela::MaterialLoader::load(std::unordered_map<std::string, Resource*>* resources, std::string src) {
	Material* material = nullptr;
	std::string line;
	std::ifstream in;
	if (!ResourceLoader::open(in, src)) {
		return false;
	}

	// This begins reading the file.
	if (in.is_open()) {
		while (std::getline(in, line)) {
			if (line.find("newmtl ") != std::string::npos) {
				material = new Material();
				std::string name = line.substr(7, line.size() - 7);
				(*resources)[name] = material;
			} else if (line.find("map_Kd ") != std::string::npos || line.find("map_Ka ") != std::string::npos) {
				// If this part of the code is reached, the line may look like "map_Kd tire_1.dds".
				std::string fileName = line.substr(7, line.size() - 7);

				// This retrieves the folder path.
				std::string path = "/";
				for (int i = src.length() - 1; i >= 0; i--) {
					if (src.at(i) == '/') {
						path = src.substr(0, i + 1);
						break;
					}
				}

				std::ifstream textureStream;
				if (!ResourceLoader::open(textureStream, path + fileName)) {
					AelaErrorHandling::windowError("Aela Material Loader", "Could not find the texture " + fileName + " inside of " + path
						+ " that was spedified in " + src + ".");

					continue;
				}

				GLuint texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				loadTexture(textureStream, &texture, GL_TEXTURE_2D, NULL, NULL);
				if (material != nullptr) {
					material->setTexture(texture);
				} else {
					// Error! The file did not specify a material name before setting the texture!
					return false;
				}
				textureStream.close();
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