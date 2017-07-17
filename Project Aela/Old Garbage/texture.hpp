#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include <string>
#include "../3D/Texture/Texture.h"
#include "../3D/Skybox/Skybox.h"

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// Load a .BMP file using our custom loader
// GLuint loadBMP_custom(const char* imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDSToGLuint(std::string filePath);
Texture loadDDSToTexture(std::string filePath);
void loadDDSToCubemapSide(GLuint* cubeMap, int side, std::string filePath);
GLuint loadBMPToGLuint(std::string filePath);
void loadSkybox(Skybox* skybox, std::string paths[6], int width, int height);

// This function should probably be moved somewhere else. Maybe it should be moved into
// a header file called "useful_functions.h". For now, it exists here.
std::string getPartOfString(std::string string, unsigned int start, unsigned int length);

#endif
