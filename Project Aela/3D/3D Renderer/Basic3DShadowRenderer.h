/*
* Name: Project Aela's Basic 3D Shadow Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D Renderer to render shadow maps for 3D objects.
*/

#pragma once
#include "../../3D/Models/Models.h"
#include "../3D Light/Light3D.h"

class Basic3DShadowRenderer {
	public:
		Basic3DShadowRenderer() {

		}

		// This adds a model's shadow to the shadow map.
		void renderShadow(Model3D* model, GLuint depthProgramID, GLuint shadowModelMatrixID, GLuint shadowMatrixID, std::vector<Light3D>* lights, GLuint lightPositionsID);
		void clearShadowMaps(std::vector<Light3D>* lights);
		unsigned int getDepthTextureWidth();
		unsigned int getDepthTextureHeight();
		void renderTestCube();

	private:
		const unsigned int DEPTH_TEXTURE_WIDTH = 2048, DEPTH_TEXTURE_HEIGHT = 2048;
		const unsigned int MAX_LIGHT_AMOUNT = 2;
};