// This is a header-only class for billboards.

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

class AelaBillboard {
	public:
		AelaBillboard() {

		}

		AelaBillboard(glm::vec3 setPosition, GLuint setTexture) {
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
			texture = loadDDS(path);
		}

	private:
		glm::vec3 position;
		GLuint texture;
};