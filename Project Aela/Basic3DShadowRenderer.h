/*
* Name: Project Aela's Basic 3D Shadow Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D Renderer to render shadow maps for 3D objects.
*/

#pragma once
#include <glm/gtc/matrix_transform.hpp>
#ifndef AELA_MODEL
#define AELA_MODEL
#include "Models.h"
#endif

class Basic3DShadowRenderer {
	public:
		Basic3DShadowRenderer() {

		}

		// This adds a model's shadow to the shadow map.
		void renderShadows(Model3D* model, GLuint depthProgramID, GLuint depthMatrixID, GLuint frameBuffer);
	};
