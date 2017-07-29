/*
* Name: Project Aela's Basic 3D Shadow Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D Renderer to render shadow maps for Transformables.
*/

#pragma once
#include "../../3D/Entity/Entity3D.h"
#include "../Light/Light3D.h"

class Basic3DShadowRenderer {
	public:
		Basic3DShadowRenderer() {

		}

		// This adds a model's shadow to the shadow map.
		void renderShadow(Entity3D* model, GLuint depthProgramID, GLuint shadowModelMatrixID, GLuint shadowMatrixID, std::vector<Light3D>* lights, GLuint lightPositionsID);

		// This clears all shadow maps of lights.
		void clearShadowMaps(std::vector<Light3D>* lights);

		// These are getters and setters.
		unsigned int getDepthTextureWidth();
		unsigned int getDepthTextureHeight();

		// This is for debugging.
		void renderTestCube();

	private:
		const unsigned int DEPTH_TEXTURE_WIDTH = 2048, DEPTH_TEXTURE_HEIGHT = 2048;
		const unsigned int MAX_LIGHT_AMOUNT = 2;
};