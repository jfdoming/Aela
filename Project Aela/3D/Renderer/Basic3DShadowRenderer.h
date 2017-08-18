/*
* Name: Project Aela's Basic 3D Shadow Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D Renderer to render shadow maps for Transformables.
*/

#pragma once
#include "../Models/ModelEntity.h"
#include "../Light/LightEntity.h"

#include <unordered_map>

using namespace Aela;

class Basic3DShadowRenderer {
	public:
		Basic3DShadowRenderer() {

		}

		// This adds a model's shadow to the shadow map.
		void renderShadow(ModelEntity* entity, GLuint depthProgramID, GLuint shadowModelMatrixID, GLuint shadowMatrixID, std::unordered_map<int, LightEntity>* lights, GLuint lightPositionsID);

		// This clears all shadow maps of lights.
		void clearShadowMaps(std::unordered_map<int, LightEntity>* lights);

		// These are getters and setters.
		unsigned int getDepthTextureWidth();
		unsigned int getDepthTextureHeight();

		// This is for debugging.
		void renderTestCube();

	private:
		const unsigned int DEPTH_TEXTURE_WIDTH = 2048, DEPTH_TEXTURE_HEIGHT = 2048;
		const unsigned int MAX_LIGHT_AMOUNT = 2;
};