/*
* Name: Project Aela's Basic 3D Shadow Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D Renderer to render shadow maps for 3D objects.
*/

#pragma once
#include "../../3D/Models/Models.h"
#include "../3D Light/Light3D.h"
#include <glm/gtc/matrix_transform.hpp>

class Basic3DShadowRenderer {
	public:
		Basic3DShadowRenderer() {

		}

		// This adds a model's shadow to the shadow map.
		void renderShadows(Model3D* model, GLuint depthProgramID, GLuint depthMatrixID, GLuint frameBuffer, std::vector<Light3D> lights);
	};
