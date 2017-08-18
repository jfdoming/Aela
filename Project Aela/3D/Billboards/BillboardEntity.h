/*
* Name: Project Aela's BillboardEntity class.
* Author: Robert Ciborowski
* Date: October 2016
* Description: A simple, header-only class used to store properties of a billboard.
*/

#pragma once
#include "../../Old Garbage/texture.hpp"
#include "../Transformable/Transformable3D.h"
#include "../../Entities/Entity.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

using namespace Aela;

class BillboardEntity : public Transformable3D, public Entity {
	public:
		BillboardEntity() {

		}

		BillboardEntity(glm::vec3 setPosition, GLuint setTexture) {
			position = setPosition;
			texture = setTexture;
		}

		void setTexture(GLuint setTexture) {
			texture = setTexture;
		}

		GLuint getTexture() {
			return texture;
		}

		EntityType getEntityType() {
			return EntityType::BILLBOARD;
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