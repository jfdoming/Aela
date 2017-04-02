/*
* Name: Project Aela's Billboard class.
* Author: Robert Ciborowski
* Date: October 2016
* Description: A simple, header-only class used to store properties of a billboard.
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

		// These are setters and getters.
		void setPosition(glm::vec3 setPosition) {
			position = setPosition;
		}

		void setTexture(GLuint setTexture) {
			texture = setTexture;
		}

		glm::vec3 getPosition() {
			return position;
		}

		void getPosition(float* setXPosition, float* setYPosition, float* setZPosition) {
			*setXPosition = position.x;
			*setYPosition = position.y;
			*setZPosition = position.z;
		}

		void setPosition(float setX, float setY, float setZ) {
			position.x = setX;
			position.y = setY;
			position.z = setZ;
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