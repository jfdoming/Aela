/*
* Name: Project Aela's Billboard class.
* Author: Ekkon Games
* Date: October 2016
* Description: A header-only class used to store properties of a billboard.
*/

#pragma once
#include "texture.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

class Billboard {
	public:
		Billboard() {

		}

		Billboard(glm::vec3 setPosition, GLuint setTexture) {
			position = setPosition;
			texture = setTexture;
		}

		void setPosition(glm::vec3 setPosition) {
			position = setPosition;
		}

		void setTexture(GLuint setTexture) {
			texture = setTexture;
		}

		glm::vec3 getPosition() {
			return position;
		}

		GLuint getTexture() {
			return texture;
		}

		void loadTexture(std::string path) {
			texture = loadDDSToGLuint(path);
		}

	private:
		glm::vec3 position;
		GLuint texture;
};