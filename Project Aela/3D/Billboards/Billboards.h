/*
* Name: Project Aela's Billboard class.
* Author: Robert Ciborowski
* Date: October 2016
* Description: A simple, header-only class used to store properties of a billboard.
*/

#pragma once
#include "../../Old Garbage/texture.hpp"
#include "../3D Object/Object3D.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

class Billboard : public Object3D {
	public:
		Billboard() {

		}

		Billboard(glm::vec3 setPosition, GLuint setTexture) {
			position = setPosition;
			texture = setTexture;
		}

		void setTexture(GLuint setTexture) {
			texture = setTexture;
		}

		GLuint getTexture() {
			return texture;
		}

		void loadTexture(std::string path) {
			texture = loadDDSToGLuint(path);
		}

		void useSpecifiedRotation(bool use) {
			useRotation = use;
		}

		bool usingSpecifiedRotation() {
			return useRotation;
		}

	private:
		GLuint texture;
		bool useRotation = true;
};